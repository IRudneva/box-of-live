#pragma once
#include "client_packet.h"
#include <optional>
#include <memory>

struct ClientPacketBuilder
{
	static std::shared_ptr<client_packet::ClientPacket> getPacket(PacketType type, const std::vector<uint8_t>& data);
};

struct ServerPacketBuilder
{
	static std::shared_ptr<Packet> getPacket(PacketType type, const std::vector<uint8_t>& data);
};

class NetworkPacketReader
{
public:
	size_t readNetworkPacket(uint8_t** data, size_t size);

	PacketType getPacketType() const { return packet_type_; }

	std::vector<uint8_t> getData();

	bool isAllDataComplete() const;

private:
	std::optional<PacketHeader> header_;
	PacketType packet_type_;
	std::vector<uint8_t> header_raw_data_;
	std::vector<uint8_t> raw_data_;

	void reset();
};

