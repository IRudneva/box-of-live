#include "pch_server.h"
#include "client_packet.h"
#include "srv_manager.h"
#include "packet_writer.h"  
#include "network_server.h"
#include "log_duration.h"

#include <iostream>

#include "server_packet.h"

void SrvManager::handlePacket(const client_packet::PacketWithIdChannel& packet)
{
	switch (packet.packet->type)
	{
	case PacketType::CLI_CREATE_ROOM:
	{
		std::cout << "room " << last_id_room_ << " created." << std::endl;
		auto pt_cl = std::static_pointer_cast<client_packet::PTCreateRoom>(packet.packet);
		FieldStateInfo game_field_state;
		game_field_state.init(pt_cl->game_config);
		rooms_state_.insert({ last_id_room_, game_field_state });

		const server_packet::PTNewRoom pt_new_room(last_id_room_, "room", game_field_state.getGameConfig());
		NetworkServer::getInstance().sendPacketAllClients(pt_new_room);
		++last_id_room_;
		break;
	}
	case PacketType::CLI_CLOSE_ROOM:
	{
		auto pt_cl = std::static_pointer_cast<client_packet::PTCloseRoom>(packet.packet);
		std::cout << "room " << pt_cl->id_room <<" close." << std::endl;
	
		for (auto it = rooms_state_.begin(); it != rooms_state_.end();)
		{
			if (it->first == pt_cl->id_room)
				it = rooms_state_.erase(it);
			else
				++it;
		}

		server_packet::PTCloseRoom pt_close_room(static_cast<uint32_t>(pt_cl->id_room));
		NetworkServer::getInstance().sendPacketAllClients(pt_close_room);

		break;
	}
	case PacketType::CLI_GET_ROOM_LIST:
	{
		std::cout << "i received PTGetRoomList" << std::endl;

		server_packet::PTRoomList pt_room_list;

		for (const auto& [id_room, state] : rooms_state_)
		{
			pt_room_list.room_list.emplace_back(static_cast<uint32_t>(id_room), "room", state.getGameConfig());
		}

		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_room_list);
		break;
	}
	case PacketType::CLI_CHOOSE_ROOM:
	{
		std::cout << "i received ChooseRoom" << std::endl;

		auto pt_choose_room = std::static_pointer_cast<client_packet::PTChooseRoom>(packet.packet);
		
		for(auto& [id_room, state] : rooms_state_)
		{
			state.deleteSubscription(static_cast<int>(packet.id_channel));
		}
		rooms_state_.at(static_cast<int>(pt_choose_room->id_room)).addSubscription(static_cast<int>(packet.id_channel));
	
		const server_packet::PTInitChooseRoom pt_init_game(pt_choose_room->id_room, rooms_state_.at(static_cast<int>(pt_choose_room->id_room)).getGameConfig());
		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_init_game);
		break;
	}
	case PacketType::CLI_START_GAME:
	{
		std::cout << "i received PTStartGame" << std::endl;
		auto pt_st = std::static_pointer_cast<client_packet::PTStartGame>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).init(pt_st->game_config);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).reset();
			std::cout << "new config for room: " << pt_st->id_room << std::endl;
		std::cout << "eb " << pt_st->game_config->energy_base << std::endl;
		std::cout << "eac " << pt_st->game_config->energy_action_cost << std::endl;
		std::cout << "efg " << pt_st->game_config->energy_from_grass << std::endl;
		std::cout << "etc " << pt_st->game_config->energy_to_clone << std::endl;
		std::cout << "ut " << pt_st->game_config->update_time << std::endl;
		std::cout << "gut " << pt_st->game_config->grass_update_time << std::endl;
		break;
	}
	case PacketType::MSG_DISABLE:
	{
		std::cout << "i received MSG DISABLE" << std::endl;
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
	if (NetworkServer::getInstance().getConnectionCount() == 0)
		return;

	if (send_field_state_timer_.timedOut())
	{
		for (auto&[room, state] : rooms_state_)
		{
			std::string log = getCurrentDatetime() + " update game state " + "room " + std::to_string(room) + "t :   ";
			LOG_DURATION(log);
			state.update();
		}

		for (const auto&[id_room, fields_state] : rooms_state_)
		{
			if(fields_state.getSubscription().empty())
				continue;

			const server_packet::PTRoomState state(id_room,
				fields_state.getCellInfo(),
				fields_state.getBacteriumInfo());

			for (const auto& client : fields_state.getSubscription())
			{
				std::cout << "send state cli:  " << client << "  id room:  " << id_room << std::endl;
				NetworkServer::getInstance().sendPacket(static_cast<uint32_t>(client), state);
			}
		}
	}
}