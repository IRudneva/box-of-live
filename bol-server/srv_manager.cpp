#include "srv_manager.h"
#include <iostream>
#include "packet_writer.h"         //////////////////////////////////////

void SrvManager::handlePacket(std::shared_ptr<DeserializePacket> packet, const hv::SocketChannelPtr& channel)
{
	DeserializePacketWriter writer;
	std::shared_ptr<DeserializePacket> cur_packet = nullptr;

	packet_queue_->pushPacket(packet);
	cur_packet = packet_queue_->popPacket();

	switch (cur_packet->type)
	{
	case PacketType::PT_CREATE_ROOM:
	{
		DeserializePacket& c = *cur_packet;
		auto p_p = dynamic_cast<PTCreateRoom&>(c);
		std::cout << "i received PTCreateRoom" << std::endl;
		int id_room = p_p.room.getId();
		Room room = p_p.room;
		room_list_.insert({ id_room, room });
		std::cout << "room " << id_room << " name " << room.name << " created." << std::endl;
		break;
	}
	case PacketType::PT_CLOSE_ROOM:
	{
		DeserializePacket& c = *cur_packet;
		auto p_p = dynamic_cast<PTCloseRoom&>(c);
		std::cout << "i received PTCloseRoom" << std::endl;

		break;
	}
	case PacketType::PT_GET_ROOM_LIST:
	{
			DeserializePacket& c = *cur_packet;
			auto p_p = dynamic_cast<PTGetRoomList&>(c);
			std::cout << "i received PTGetRoomList" << std::endl;
			std::cout << "i form a room list..." << std::endl;
			std::shared_ptr<PTRoomList> pt_room_list = std::make_shared<PTRoomList>();
			for (const auto[id, room] : room_list_)
			{
				pt_room_list->room_list.push_back(room);
			}
			writer.writeDeserializePacket(pt_room_list);
			auto net_pac = writer.getSerializePacket();
			channel->write((uint8_t*)&net_pac->header, (int)sizeof(net_pac->header));
			channel->write(net_pac->data.data(), (int)net_pac->data.size());
	

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
