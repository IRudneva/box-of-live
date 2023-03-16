
#pragma once
#include <optional>

#include "domain.h"

enum class PacketType : uint32_t
{
	PT_MSG_CREATE_ROOM,
	PT_MSG_GET_ROOM_LIST,
	PT_ROOM_LIST
};

struct PacketHeader
{
	PacketType  packet_type;
	uint32_t data_size;
};

struct Packet
{
	PacketHeader header;
	std::vector<uint8_t> data;

	template<class T>
		void pack(T& packer) {
			packer(data);
		}
};

class PacketReader
{
public:
	size_t readData(uint8_t** data, size_t size);

	Packet getPacket();

	bool isAllDataComplete();

	void reset();


	std::optional<PacketHeader> header;

	std::vector<uint8_t> header_raw_data;
	std::vector<uint8_t> raw_data;
};


class PacketWriter
{
public:
	void writePacket(std::shared_ptr<Packet> packet);

	std::vector<uint8_t> getData();

	void reset();

	std::shared_ptr<Packet> packet_;
};

