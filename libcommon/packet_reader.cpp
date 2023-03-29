#include "pch.h"
#include "msgpack.hpp"
#include "packet_type.h"
#include "client_packet.h"
#include "server_packet.h"
#include "packet_reader.h"

std::shared_ptr<client_packet::ClientPacket> ClientPacketBuilder::getPacket(PacketType type, const std::vector<uint8_t>& data)
{
	switch (type)
	{
	case PacketType::CLI_CREATE_ROOM:
	{
		std::shared_ptr<client_packet::PTCreateRoom> pt_create_room = std::make_shared<client_packet::PTCreateRoom>();
		auto packet = msgpack::unpack<client_packet::PTCreateRoom>(data);
		*pt_create_room = packet;

		auto result = std::static_pointer_cast<client_packet::ClientPacket>(pt_create_room);
		return result;
	}
	case PacketType::CLI_CLOSE_ROOM:
	{
		std::shared_ptr<client_packet::PTCloseRoom> pt_close_room = std::make_shared<client_packet::PTCloseRoom>();
		auto packet = msgpack::unpack<client_packet::PTCloseRoom>(data);
		*pt_close_room = packet;
		auto result = std::static_pointer_cast<client_packet::ClientPacket>(pt_close_room);
		return result;
	}
	case PacketType::CLI_GET_ROOM_LIST:
	{
		std::shared_ptr<client_packet::PTGetRoomList> pt_get_room = std::make_shared<client_packet::PTGetRoomList>();
		auto packet = msgpack::unpack<client_packet::PTGetRoomList>(data);
		*pt_get_room = packet;
		auto result = std::static_pointer_cast<client_packet::ClientPacket>(pt_get_room);
		return result;
	}
	default:
		return nullptr;
	}
}

std::shared_ptr<server_packet::ServerPacket> ServerPacketBuilder::getPacket(PacketType type, const std::vector<uint8_t>& data)
{
	switch (type)
	{
	case PacketType::SRV_NEW_ROOM:
	{
		std::shared_ptr<server_packet::PTNewRoom> pt_new_room = std::make_shared<server_packet::PTNewRoom>();
		auto packet = msgpack::unpack<server_packet::PTNewRoom>(data);
		*pt_new_room = packet;
		auto result = std::static_pointer_cast<server_packet::ServerPacket>(pt_new_room);
		return result;
	}
	case PacketType::SRV_ROOM_LIST:
	{
		std::shared_ptr<server_packet::PTRoomList> pt_room_list = std::make_shared<server_packet::PTRoomList>();
		auto packet = msgpack::unpack<server_packet::PTRoomList>(data);
		*pt_room_list = packet;
		auto result = std::static_pointer_cast<server_packet::ServerPacket>(pt_room_list);
		return result;
	}
	default:
		return nullptr;
	}
}

std::vector<uint8_t> NetworkPacketReader::getData()
{
	auto buffer = raw_data_;
	reset();
	return buffer;
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
	packet_type_ = header_.value().packet_type;
	size_t sizeLeft = header_->data_size - raw_data_.size();
	size_t sizeToRead = std::min(size, sizeLeft);

	for (size_t i = 0; i < sizeToRead; i++) {
		raw_data_.push_back(funcReadOneByte());
	}

	return size;
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

