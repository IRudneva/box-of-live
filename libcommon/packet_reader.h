//#pragma once
#include <optional>

enum class PacketType : uint32_t
{
	PT_MSG_GET_ROOM_LIST,
	PT_ROOM_LIST
};
//
//
//struct Packet
//{
//	PacketHeader header;
//	std::vector<uint8_t> data;
//};

class PacketReader
{
public:
	size_t readData(uint8_t** data, size_t size);

	bool isAllDataComplete();

	void reset();

	struct PacketHeader
	{
		PacketType  packet_type;
		uint32_t data_size;
	};

	std::optional<PacketHeader> header;

	std::vector<uint8_t> header_raw_data;
	std::vector<uint8_t> raw_data;
};