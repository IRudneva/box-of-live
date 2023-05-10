#include "pch_server.h"
#include "client_packet.h"
#include "srv_manager.h"

#include "grass.h"
#include "packet_writer.h"  
#include "network_server.h"
#include "room_state.h"

#include "logger.h"
#include "server_packet.h"

void SrvManager::initState(const std::map<int, DbRoomInfo>& rooms_state, const std::map<int, std::unordered_map<XYPos, DbCellState, pairhash>>& cell_inf, const std::vector<DbBacteriumColorState>& bact_inf)
{
	for (const auto&[id_room, room_info] : rooms_state)
	{
		std::map<int, SrvColor> color_map;
		for (const auto& col : bact_inf)
		{
			if (id_room == col.id_room)
			{
				color_map.insert({ col.id_type, SrvColor(col.red, col.green, col.blue) });
			}
		}

		auto state = createRoom(id_room, room_info.is_active, room_info.config, color_map);

		if (cell_inf.find(id_room) == cell_inf.end())
		{
			rooms_state_.insert({ id_room, state });
			last_id_room_ = id_room;
			continue;
		}

		for(auto i = cell_inf.at(id_room).begin(); i != cell_inf.at(id_room).end(); ++i)
		{
			switch (static_cast<TypeCell>(i->second.cell_type))
			{
			case TypeCell::GRASS:
			{
				if (i->second.is_super_grass)
					state.addSuperGrass(i->first.first, i->first.second);
				else
					state.addGrass(i->first.first, i->first.second);
				break;
			}
			case TypeCell::BACTERIUM:
			{
				state.addBacterium(i->first.first, i->first.second, i->second.bact_type, i->second.energy);
				break;
			}
			case TypeCell::EMPTY: break;
			default: break;
			}
		}

		rooms_state_[id_room] = state;
		last_id_room_ = id_room;
	}
	if (!rooms_state.empty())
		++last_id_room_;
}

RoomState SrvManager::createRoom(int id, bool status, const GameConfig& config,
                                 const std::map<int, SrvColor>& color_bacterium) const
{
	RoomState state(id, status);
	state.initConfig(std::make_shared<GameConfig>(config));
	state.setColorByBacteriumMap(color_bacterium);
	return state;
}

RoomState SrvManager::createRoom(int id, bool status, std::shared_ptr<GameConfig> config) const
{
	RoomState state(id, status);
	state.initConfig(config);
	state.initBacteriumColors(config->count_colonies);
	return state;
}

void SrvManager::handlePacket(const client_packet::PacketWithIdChannel& packet)
{
	switch (packet.packet->type)
	{
	case PacketType::CLI_CREATE_ROOM:
	{
		srand(static_cast<unsigned int>(time(NULL)));
		auto pt_cl = std::static_pointer_cast<client_packet::PTCreateRoom>(packet.packet);

		auto new_room_state = createRoom(last_id_room_, false, pt_cl->game_config);
	
		rooms_state_.insert({ last_id_room_, new_room_state });

		Logger::getInstance()->registerLog("SERVER::RECEIVED::CREATE ROOM::" + std::to_string(last_id_room_));

		const server_packet::PTNewRoom pt_new_room(last_id_room_, "room", pt_cl->game_config);
		NetworkServer::getInstance()->sendPacketAllClients(pt_new_room);

		DbPayload::getInstance()->updateRoomsConfigInfo(last_id_room_, DbRoomInfo(false, *pt_cl->game_config));

		std::vector<DbBacteriumColorState> color_state;
		for (const auto&[id_type, color] : new_room_state.getColorsBacterium())
		{
			color_state.emplace_back(last_id_room_, id_type, color.red, color.green, color.blue);
		}
		DbPayload::getInstance()->updateBacteriumColorStates(color_state);
		++last_id_room_;

		break;
	}
	case PacketType::CLI_CLOSE_ROOM:
	{
		auto pt_cl = std::static_pointer_cast<client_packet::PTCloseRoom>(packet.packet);

		Logger::getInstance()->registerLog("SERVER::RECEIVED::CLOSE ROOM::" + std::to_string(pt_cl->id_room));
	
		for (auto it = rooms_state_.begin(); it != rooms_state_.end();)
		{
			if (it->first == pt_cl->id_room)
				it = rooms_state_.erase(it);
			else
				++it;
		}

		server_packet::PTCloseRoom pt_close_room(pt_cl->id_room);
		NetworkServer::getInstance()->sendPacketAllClients(pt_close_room);

		DbSaveRoomState state = { static_cast<int>(pt_close_room.id_room) , true };
		DbPayload::getInstance()->updateCellsRoomState(state);
		
		break;
	}
	case PacketType::CLI_GET_ROOM_LIST:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::GET ROOM LIST");

		server_packet::PTRoomList pt_room_list;
		for (const auto& [id_room, state] : rooms_state_)
		{
			pt_room_list.room_list.emplace_back(static_cast<uint32_t>(id_room), "room", state.getGameConfig());
		}

		NetworkServer::getInstance()->sendPacket(packet.id_channel, pt_room_list);
		break;
	}
	case PacketType::CLI_CHOOSE_ROOM:
	{
		auto pt_choose_room = std::static_pointer_cast<client_packet::PTChooseRoom>(packet.packet);

		Logger::getInstance()->registerLog("SERVER::RECEIVED::CHOOSE ROOM::" + std::to_string(pt_choose_room->id_room));

		for (auto&[id_room, state] : rooms_state_)
		{
			state.deleteSubscription(static_cast<int>(packet.id_channel));
		}
		rooms_state_.at(static_cast<int>(pt_choose_room->id_room)).addSubscription(static_cast<int>(packet.id_channel));

		auto current_room = rooms_state_.at(static_cast<int>(pt_choose_room->id_room));

		sendCurrentState(current_room, static_cast<int>(packet.id_channel));

		break;
	}
	case PacketType::CLI_START_GAME:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::START GAMES");

		auto pt_st = std::static_pointer_cast<client_packet::PTStartGame>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).initConfig(pt_st->game_config);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).setActivateStatus(true);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).initBacteriumColors(pt_st->game_config->count_colonies);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).reset();

		DbRoomInfo info = { true,  *pt_st->game_config };
		DbPayload::getInstance()->updateRoomsConfigInfo(static_cast<int>(pt_st->id_room), info);

		auto current_room = rooms_state_.at(static_cast<int>(pt_st->id_room));

		for (const auto& client : current_room.getSubscription())
		{
			sendCurrentState(current_room, client);
		}

		break;
	}
	case PacketType::CLI_ADD_SUPER_GRASS:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::ADD SUPER GRASS");
		auto pt_st = std::static_pointer_cast<client_packet::PTAddSuperGrass>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).addSuperGrass(static_cast<int>(pt_st->pos_x), static_cast<int>(pt_st->pos_y));
		break;
	}
	case PacketType::CLI_ADD_EFFECT:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::ADD EFFECT");
		auto pt_st = std::static_pointer_cast<client_packet::PTAddEffect>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).addEffect(static_cast<int>(pt_st->pos_x), static_cast<int>(pt_st->pos_y));
		break;
	}
	case PacketType::MSG_DISABLE:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::MSG DISABLE");

		auto pt_st = std::static_pointer_cast<ConnectionMessage>(packet.packet);
		
		for (auto&[id_room, state] : rooms_state_)
		{
			state.deleteSubscription(static_cast<int>(packet.id_channel));
		}
			
		break;
	}
	default:
	{
		break;
	}
	}
}

void SrvManager::updateGameState()
{
	for (const auto& [room, state] : rooms_state_)
	{
		state.update();
	}
}

void SrvManager::sendCurrentState(const RoomState& current_room, int client) const
{
	auto current_state = current_room.getAllCellInfo();

	const auto current_config = current_room.getGameConfig();

	const auto bacterium_color = current_room.getColorsBacterium();

	const auto current_status = current_room.getStatus();

	std::vector<GrassInfo> grass_info;
	std::vector<BacteriumInfo> bacterium_info;

	for (const auto&[id, cell] : current_state)
	{
		auto pos = cell->getPosition();

		if (cell->getCellType() == TypeCell::BACTERIUM)
		{
			const auto bacterium = std::dynamic_pointer_cast<Bacterium>(cell);
			BacteriumInfo inf_bac(pos.x, pos.y, bacterium->getIdType(), bacterium->getEnergy());
			bacterium_info.emplace_back(inf_bac);
		}
		else if (cell->getCellType() == TypeCell::GRASS)
		{
			const auto grass = std::dynamic_pointer_cast<Grass>(cell);
			GrassInfo inf_grass(pos.x, pos.y, grass->isSuperGrass());
			grass_info.emplace_back(inf_grass);
		}
	}

	const server_packet::PTInitChooseRoom pt_init_game(
		current_room.getIdRoom(),
		current_status,
		grass_info,
		bacterium_info,
		current_config,
		bacterium_color
	);

	NetworkServer::getInstance()->sendPacket(static_cast<uint32_t>(client), pt_init_game);
}