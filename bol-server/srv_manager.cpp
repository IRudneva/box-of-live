#include "pch_server.h"
#include "client_packet.h"
#include "srv_manager.h"
#include "packet_writer.h"  
#include "network_server.h"

#include <iostream>

#include "server_packet.h"

void SrvManager::handlePacket(const client_packet::PacketWithIdChannel& packet)
{
	switch (packet.packet->type)
	{
	case PacketType::CLI_CREATE_ROOM:
	{
		std::cout << "i received PTCreateRoom" << std::endl;

		auto new_room = room_list_.insert({ last_id_room_, Room((uint32_t)last_id_room_, "room") });
		std::cout << "room " << new_room.first->first << " created." << std::endl;
		////////////////////////////////////////////
		FieldStateInfo game_field_state;
		rooms_state_.insert({ last_id_room_, game_field_state });
			////////////////////////////////
		server_packet::PTNewRoom pt_new_room(new_room.first->second);
		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_new_room);
		++last_id_room_;
		break;
	}
	case PacketType::CLI_CLOSE_ROOM:
	{
		auto pt_cl = std::static_pointer_cast<client_packet::PTCloseRoom>(packet.packet);
		std::cout << "i received PTCloseRoom" << std::endl;
		room_list_.erase(pt_cl->id_room);
		break;
	}
	case PacketType::CLI_GET_ROOM_LIST:
	{
		std::cout << "i received PTGetRoomList" << std::endl;
		std::cout << "i form a room list..." << std::endl;

		server_packet::PTRoomList pt_room_list;
		
		for (const auto& [id, room] : room_list_)
		{
			pt_room_list.room_list.push_back(room);
		}

		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_room_list);
		break;
	}
	case PacketType::CLI_GET_ROOM_STATE:
	{
		std::cout << "i received PTGetRoomSTATE" << std::endl;
		auto pt_rst = std::static_pointer_cast<client_packet::PTGetRoomState>(packet.packet);
		const server_packet::PTRoomState pt_room_state(pt_rst->id_room,
			rooms_state_.at(static_cast<int>(pt_rst->id_room)).getCellInfo(),
			rooms_state_.at(static_cast<int>(pt_rst->id_room)).getBacteriumInfo());
		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_room_state);
		break;
	}
	case PacketType::CLI_CHANGE_CONFIG:
	{
		std::cout << "i received ChangeConfig" << std::endl;
		auto pt_change_config = std::static_pointer_cast<client_packet::PTChangeConfig>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_change_config->id_room)).init(pt_change_config->game_config);
		{//test
			std::cout << "new config for room: " << pt_change_config->id_room << std::endl;
			std::cout << "eb " << pt_change_config->game_config->energy_base << std::endl;
			std::cout << "eac " << pt_change_config->game_config->energy_action_cost << std::endl;
			std::cout << "efg " << pt_change_config->game_config->energy_from_grass << std::endl;
			std::cout << "etc " << pt_change_config->game_config->energy_to_clone << std::endl;
			std::cout << "ut " << pt_change_config->game_config->update_time << std::endl;
			std::cout << "gut " << pt_change_config->game_config->grass_update_time << std::endl;
		}
		break;
	}
	case PacketType::CLI_START_GAME:
	{
		std::cout << "i received PTStartGame" << std::endl;
		auto pt_st = std::static_pointer_cast<client_packet::PTStartGame>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).reset();
		const server_packet::PTRoomState pt_room_state(pt_st->id_room,
			rooms_state_.at(static_cast<int>(pt_st->id_room)).getCellInfo(),
			rooms_state_.at(static_cast<int>(pt_st->id_room)).getBacteriumInfo());
		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_room_state);
		break;
	}
	default:
	{
		break;
	}
	}
}
