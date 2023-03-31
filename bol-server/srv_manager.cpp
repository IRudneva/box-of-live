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
		const server_packet::PTRoomState pt_room_state(pt_rst->id_room);
		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_room_state);
		break;
	}
	default:
	{
		break;
	}
	}
}
