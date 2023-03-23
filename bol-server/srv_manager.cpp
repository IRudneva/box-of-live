#include "pch_server.h"
#include "srv_manager.h"
#include "packet_writer.h"  
#include "network_server.h"

#include <iostream>

void SrvManager::handlePacket(const DeserializePacketWithIdChannel& packet)
{
	DeserializePacketWriter writer;
	DeserializePacket& pt_ds = *packet.packet;

	switch (packet.packet->type)
	{
	case PacketType::PT_CREATE_ROOM:
	{
		auto pt_cr = dynamic_cast<PTCreateRoom&>(pt_ds);
		std::cout << "i received PTCreateRoom" << std::endl;
		int id_room = pt_cr.room.getId();
		Room room = pt_cr.room;
		room_list_.insert({ id_room, room });
		std::cout << "room " << id_room << " name " << room.name << " created." << std::endl;
		break;
	}
	case PacketType::PT_CLOSE_ROOM:
	{
		auto pt_cl = dynamic_cast<PTCloseRoom&>(pt_ds);
		std::cout << "i received PTCloseRoom" << std::endl;
		room_list_.erase(pt_cl.id_room);
		break;
	}
	case PacketType::PT_GET_ROOM_LIST:
	{
		auto pt_grl = dynamic_cast<PTGetRoomList&>(pt_ds);
		std::cout << "i received PTGetRoomList" << std::endl;
		std::cout << "i form a room list..." << std::endl;

		std::shared_ptr<PTRoomList> pt_room_list = std::make_shared<PTRoomList>();
		
		for (const auto& [id, room] : room_list_)
		{
			pt_room_list->room_list.push_back(room);
		}
		DeserializePacketWithIdChannel ds_packet;
		std::shared_ptr<DeserializePacket> data = std::static_pointer_cast<DeserializePacket>(pt_room_list);
		ds_packet.packet = data;
		ds_packet.id_channel = packet.id_channel;

		auto s_packet = writer.getSerializePacket(ds_packet);

		NetworkServer::sendPacket(s_packet);
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
