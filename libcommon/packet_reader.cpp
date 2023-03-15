#include "pch.h"
#include "packet_reader.h"

size_t PacketReader::readData(uint8_t** data, size_t size)
{
	const auto funcReadOneByte = [&]() {
		uint8_t ret = **data;
		(*data)++;
		size--;
		return ret;
	};

	// Read header
	if (!header) {
		size_t headerSizeExpected = sizeof(PacketHeader);
		size_t headerSizeLeft = headerSizeExpected - header_raw_data.size();

		size_t sizeToRead = std::min(size, headerSizeLeft);

		for (size_t i = 0; i < sizeToRead; i++) {
			header_raw_data.push_back(funcReadOneByte());
		}

		if (header_raw_data.size() != headerSizeExpected)
			return size;


		header = *(PacketHeader*)header_raw_data.data();
	}
	// Read body

	size_t sizeLeft = header->data_size - raw_data.size();
	size_t sizeToRead = std::min(size, sizeLeft);

	for (size_t i = 0; i < sizeToRead; i++) {
		raw_data.push_back(funcReadOneByte());
	}

	return size;
}

Packet PacketReader::getPacket()
{
	return msgpack::unpack<Packet>(raw_data);
}

bool PacketReader::isAllDataComplete()
{
	if (!header)
		return false;

	return header->data_size == raw_data.size();
}


void PacketReader::reset()
{
	header.reset();
	header_raw_data.clear();
	raw_data.clear();
}
