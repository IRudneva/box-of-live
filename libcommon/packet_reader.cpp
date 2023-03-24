#include "pch.h"
#include "msgpack.hpp"
#include "packet_type.h"
#include "client_packet.h"
#include "server_packet.h"
#include "packet_reader.h"


std::shared_ptr<ClientPacket> ClientPacketBuilder::getPacket(PacketType type, const std::vector<uint8_t>& data)
{
	switch (type)
	{
	case PacketType::PT_CREATE_ROOM:
	{
		std::shared_ptr<PTCreateRoom> pt_create_room = std::make_shared<PTCreateRoom>();
		auto packet = msgpack::unpack<PTCreateRoom>(data);
		*pt_create_room = packet;

		auto result = std::static_pointer_cast<ClientPacket>(pt_create_room);
		return result;
	}
	case PacketType::PT_CLOSE_ROOM:
	{
		std::shared_ptr<PTCloseRoom> pt_close_room = std::make_shared<PTCloseRoom>();
		auto packet = msgpack::unpack<PTCloseRoom>(data);
		*pt_close_room = packet;
		auto result = std::static_pointer_cast<ClientPacket>(pt_close_room);
		return result;
	}
	case PacketType::PT_GET_ROOM_LIST:
	{
		std::shared_ptr<PTGetRoomList> pt_get_room = std::make_shared<PTGetRoomList>();
		auto packet = msgpack::unpack<PTGetRoomList>(data);
		*pt_get_room = packet;
		auto result = std::static_pointer_cast<ClientPacket>(pt_get_room);
		return result;
	}
	default:
		return nullptr;
	}
}

std::shared_ptr<ServerPacket> ServerPacketBuilder::getPacket(PacketType type, const std::vector<uint8_t>& data)
{
	switch (type)
	{
	case PacketType::PT_ROOM_LIST:
	{
		std::shared_ptr<PTRoomList> pt_room_list = std::make_shared<PTRoomList>();
		auto packet = msgpack::unpack<PTRoomList>(data);
		*pt_room_list = packet;
		auto result = std::static_pointer_cast<ServerPacket>(pt_room_list);
		return result;
	}
	default:
		return nullptr;
	}
}


size_t NetworkPacketReader::readNetworkPacket(uint8_t** data, size_t size)
{
	const auto funcReadOneByte = [&]() {
		uint8_t ret = **data;
		(*data)++;
		size--;
		return ret;
	};

	// Read header
	if (!header_) {

		size_t header_size_expected = sizeof(PacketHeader);

		size_t header_size_left = header_size_expected - header_raw_data_.size();

		size_t sizeToRead = std::min(size, header_size_left);

		for (size_t i = 0; i < sizeToRead; i++) {
			header_raw_data_.push_back(funcReadOneByte());
		}

		if (header_raw_data_.size() != header_size_expected)
			return size;


		header_ = *(reinterpret_cast<PacketHeader*>(header_raw_data_.data()));

	}
	// Read body

	size_t sizeLeft = header_->data_size - raw_data_.size();
	size_t sizeToRead = std::min(size, sizeLeft);

	for (size_t i = 0; i < sizeToRead; i++) {
		raw_data_.push_back(funcReadOneByte());
	}

	return size;
}

//std::shared_ptr<Packet> NetworkPacketReader::getPacket() {
//	switch (header_.value().packet_type)
//	{
//	case PacketType::PT_CREATE_ROOM:
//	{
//		std::shared_ptr<PTCreateRoom> pt_create_room = std::make_shared<PTCreateRoom>();
//		auto packet = msgpack::unpack<PTCreateRoom>(raw_data_);
//		*pt_create_room = packet;
//
//		auto result = std::static_pointer_cast<Packet>(pt_create_room);
//		reset();
//		return result;
//	}
//	case PacketType::PT_CLOSE_ROOM:
//	{
//		std::shared_ptr<PTCloseRoom> pt_close_room = std::make_shared<PTCloseRoom>();
//		auto packet = msgpack::unpack<PTCloseRoom>(raw_data_);
//		*pt_close_room = packet;
//		auto result = std::static_pointer_cast<Packet>(pt_close_room);
//		reset();
//		return result;
//	}
//	case PacketType::PT_GET_ROOM_LIST:
//	{
//		std::shared_ptr<PTGetRoomList> pt_get_room = std::make_shared<PTGetRoomList>();
//		auto packet = msgpack::unpack<PTGetRoomList>(raw_data_);
//		*pt_get_room = packet;
//		auto result = std::static_pointer_cast<Packet>(pt_get_room);
//		reset();
//		return result;
//	}
//	case PacketType::PT_ROOM_LIST:
//	{
//		std::shared_ptr<PTRoomList> pt_room_list = std::make_shared<PTRoomList>();
//		auto packet = msgpack::unpack<PTRoomList>(raw_data_);
//		*pt_room_list = packet;
//		auto result = std::static_pointer_cast<Packet>(pt_room_list);
//		reset();
//		return result;
//	}
//	default:
//		reset();
//		return nullptr;
//	}
//}

bool NetworkPacketReader::isAllDataComplete() const
{
	if (!header_)
		return false;

	return header_->data_size == raw_data_.size();
}


void NetworkPacketReader::reset()
{
	header_.reset();
	header_raw_data_.clear();
	raw_data_.clear();
}

