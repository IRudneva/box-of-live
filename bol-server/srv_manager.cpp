#include "pch_server.h"
#include "client_packet.h"
#include "srv_manager.h"
#include "packet_writer.h"  
#include "network_server.h"
#include "log_duration.h"
#include "request_handler.h"
#include "room_state.h"

#include "server_packet.h"

void SrvManager::handlePacket(const client_packet::PacketWithIdChannel& packet)
{
	std::lock_guard<std::mutex> lock(m_);
	switch (packet.packet->type)
	{
	case PacketType::CLI_CREATE_ROOM:
	{
		auto pt_cl = std::static_pointer_cast<client_packet::PTCreateRoom>(packet.packet);

		RoomState game_field_state(last_id_room_, false);

		game_field_state.init(pt_cl->game_config);

		rooms_state_.insert({ last_id_room_, game_field_state });

		Logger::getInstance()->registerLog("SERVER::RECEIVED::CREATE ROOM::" + std::to_string(last_id_room_));

		const server_packet::PTNewRoom pt_new_room(last_id_room_, "room", game_field_state.getGameConfig());
		NetworkServer::getInstance()->sendPacketAllClients(pt_new_room);

		RequestHandler::getInstance()->saveNewRoom(last_id_room_, RoomInfo(false, *game_field_state.getGameConfig()));
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

		RequestHandler::getInstance()->deleteRoom(static_cast<int>(pt_close_room.id_room));
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

		Logger::getInstance()->registerLog("SERVER::RECEIVED::CHOOSE ROOM::"+ std::to_string(pt_choose_room->id_room));

		for(auto& [id_room, state] : rooms_state_)
		{
			state.deleteSubscription(static_cast<int>(packet.id_channel));
		}
		rooms_state_.at(static_cast<int>(pt_choose_room->id_room)).addSubscription(static_cast<int>(packet.id_channel));

		auto current_state = rooms_state_.at(static_cast<int>(pt_choose_room->id_room)).getAllCellInfo();

		auto current_config = rooms_state_.at(static_cast<int>(pt_choose_room->id_room)).getGameConfig();

		std::vector<GrassInfo> grass_info;
		std::vector<BacteriumInfo> bacterium_info;
		for(const auto& [id, cell] : current_state)
		{
			auto pos = cell->getPosition();

			if (cell->getCellType() == TypeCell::BACTERIUM)
			{
				Cell& a = *cell;
				auto bacterium = dynamic_cast<Bacterium&>(a);
				BacteriumInfo inf_bac(pos.x, pos.y, bacterium.getIdType(), bacterium.getEnergy());
				bacterium_info.emplace_back(inf_bac);
			}
			else if (cell->getCellType() == TypeCell::GRASS)
			{
				GrassInfo inf_grass(pos.x, pos.y);
				grass_info.emplace_back(inf_grass);
			}
		}

		const server_packet::PTInitChooseRoom pt_init_game(
			pt_choose_room->id_room,
			grass_info,
			bacterium_info,
			current_config
		);

		NetworkServer::getInstance()->sendPacket(packet.id_channel, pt_init_game);
		break;
	}
	case PacketType::CLI_START_GAME:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::START GAMES");

		auto pt_st = std::static_pointer_cast<client_packet::PTStartGame>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).init(pt_st->game_config);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).activate(true);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).reset();

		RequestHandler::getInstance()->changeRoomStatus(static_cast<int>(pt_st->id_room), true);
		RequestHandler::getInstance()->changeRoomConfig(static_cast<int>(pt_st->id_room), *pt_st->game_config);
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
	//std::lock_guard<std::mutex> lock(m_);
	if (NetworkServer::getInstance()->getConnectionCount() == 0)
		return;

	if (send_field_state_timer_.timedOut())
	{
		for (auto&[room, state] : rooms_state_)
		{
			state.update();
		}
	}
}