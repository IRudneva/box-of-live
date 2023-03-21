#pragma once
#include "packet_domain.h"
#include <optional>
#include <memory>

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

