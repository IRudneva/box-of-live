#pragma once
#include "client_packet.h"
#include "server_packet.h"
#include "network_packet.h"
#include <optional>
#include <memory>

struct ClientPacketBuilder
{
	static std::shared_ptr<ClientPacket> getPacket(PacketType type, const std::vector<uint8_t>& data);
};

struct ServerPacketBuilder
{
	static std::shared_ptr<ServerPacket> getPacket(PacketType type, const std::vector<uint8_t>& data);
};

class NetworkPacketReader
{
public:
	size_t readNetworkPacket(uint8_t** data, size_t size);

	PacketType getPacketType() const { if (header_.has_value()) return header_.value().packet_type; }

	std::vector<uint8_t> getData()
	{
		auto buffer = raw_data_;
		reset();
		return buffer;
	}

	//std::shared_ptr<Packet> getPacket();

	bool isAllDataComplete() const;

	//std::optional<PacketHeader> getNetworkPacketHeader() const { return header_; }

private:
	std::optional<PacketHeader> header_;
	std::vector<uint8_t> header_raw_data_;
	std::vector<uint8_t> raw_data_;
	void reset();
};

