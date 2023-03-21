#pragma once
#include <optional>
#include "domain.h"
#include "packet_domain.h"
#include "pch.h"

class NetworkPacketReader
{
public:
	size_t readNetworkPacket(uint8_t** data, size_t size);

	std::shared_ptr<DeserializePacket> getDeserializePacket();

	bool isAllDataComplete() const;

	std::optional<PacketHeader> getNetworkPacketHeader() const { return header_; }

private:
	std::optional<PacketHeader> header_;
	std::vector<uint8_t> header_raw_data_;
	std::vector<uint8_t> raw_data_;

	void reset();
};

class DeserializePacketWriter
{
public:
	template <typename T>
	void writeDeserializePacket(T deserialize_pac)
	{
		if (deserialize_pac == nullptr) return;

		des_packet_->header.packet_type = deserialize_pac->type;
		des_packet_->data = msgpack::pack(*(deserialize_pac));
		des_packet_->header.data_size = des_packet_->data.size();
	}

	std::shared_ptr<NetworkPacket> getSerializePacket();

private:
	std::shared_ptr<NetworkPacket> des_packet_ = nullptr;

	void reset() { des_packet_ = nullptr; }
};
