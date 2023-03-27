#include "pch_server.h"
#include "client_packet.h"
#include "srv_manager.h"
#include "packet_writer.h"  
#include "network_server.h"

#include <iostream>

#include "server_packet.h"

void SrvManager::handlePacket(const PacketWithIdChannel& packet)
{
	switch (packet.packet->type)
	{
	case PacketType::PT_CREATE_ROOM:
	{
		auto pt_cr = std::static_pointer_cast<PTCreateRoom>(packet.packet);
		std::cout << "i received PTCreateRoom" << std::endl;
		int id_room = pt_cr->room.getId();
		Room room = pt_cr->room;
		room_list_.insert({ id_room, room });
		std::cout << "room " << id_room << " name " << room.name << " created." << std::endl;
		break;
	}
	case PacketType::PT_CLOSE_ROOM:
	{
		auto pt_cl = std::static_pointer_cast<PTCloseRoom>(packet.packet);
		std::cout << "i received PTCloseRoom" << std::endl;
		room_list_.erase(pt_cl->id_room);
		break;
	}
	case PacketType::PT_GET_ROOM_LIST:
	{
		std::cout << "i received PTGetRoomList" << std::endl;
		std::cout << "i form a room list..." << std::endl;

		PTRoomList pt_room_list;
		
		for (const auto& [id, room] : room_list_)
		{
			pt_room_list.room_list.push_back(room);
		}
	
		NetworkServer::sendPacket(packet.id_channel, pt_room_list);
		break;
	}
	case PacketType::PT_ROOM_LIST:
	{
		std::cout << "i received PTRoomList" << std::endl;
		break;
	}
	default:
	{
		break;
	}
	}
}
