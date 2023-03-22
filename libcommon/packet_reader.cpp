#include "pch.h"
#include "msgpack.hpp"
#include "packet_reader.h"


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

DeserializePacketWithIdChannel NetworkPacketReader::getDeserializePacket() {
	DeserializePacketWithIdChannel result;

	switch (header_.value().packet_type)
	{
	case PacketType::PT_CREATE_ROOM:
	{
		std::shared_ptr<PTCreateRoom> pt_create_room = std::make_shared<PTCreateRoom>();
		auto packet = msgpack::unpack<PTCreateRoom>(raw_data_);
		*pt_create_room = packet;

		result.packet = std::static_pointer_cast<DeserializePacket>(pt_create_room);
		reset();
		return result;
	}
	case PacketType::PT_CLOSE_ROOM:
	{
	/*	std::shared_ptr<PTCloseRoom> pt_close_room = std::make_shared<PTCloseRoom>();
		auto packet = msgpack::unpack<PTCloseRoom>(raw_data_);
		*pt_close_room = packet;
		result = std::static_pointer_cast<DeserializePacket>(pt_close_room);
		reset();*/
		return result;
	}
	case PacketType::PT_GET_ROOM_LIST:
	{
		/*std::shared_ptr<PTGetRoomList> pt_get_room = std::make_shared<PTGetRoomList>();
		auto packet = msgpack::unpack<PTGetRoomList>(raw_data_);
		*pt_get_room = packet;
		result = std::static_pointer_cast<DeserializePacket>(pt_get_room);
		reset();*/
		return result;
	}
	case PacketType::PT_ROOM_LIST:
	{
		/*std::shared_ptr<PTRoomList> pt_room_list = std::make_shared<PTRoomList>();
		auto packet = msgpack::unpack<PTRoomList>(raw_data_);
		*pt_room_list = packet;
		result = std::static_pointer_cast<DeserializePacket>(pt_room_list);
		reset();*/
		return result;
	}
	default:
		reset();
		return result;
	}
}

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

