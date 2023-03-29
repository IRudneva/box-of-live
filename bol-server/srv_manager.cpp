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
		auto pt_cr = std::static_pointer_cast<client_packet::PTCreateRoom>(packet.packet);
		std::cout << "i received PTCreateRoom" << std::endl;
		int id_room = pt_cr->room.getId();
		Room room = std::move(pt_cr->room);
		auto new_room = room_list_.insert({ id_room, std::move(room) });
		std::cout << "room " << id_room << " name " << room.name << " created." << std::endl;
		server_packet::PTNewRoom pt_new_room;
		pt_new_room.room = new_room.first->second;
		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_new_room);
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
	default:
	{
		break;
	}
	}
}
