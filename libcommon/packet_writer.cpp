#include "pch.h"
#include "msgpack.hpp"
#include "packet_writer.h"

std::shared_ptr<NetworkPacket> DeserializePacketWriter::getSerializePacket() {
	auto buff_pac = des_packet_;
	reset();
	return buff_pac;
}

std::vector<uint8_t> DeserializePacketWriter::packPacket(PTCloseRoom p) const
{
	return  msgpack::pack(p);
}
std::vector<uint8_t> DeserializePacketWriter::packPacket(PTCreateRoom p) const 
{
	return  msgpack::pack(p);
}
std::vector<uint8_t> DeserializePacketWriter::packPacket(PTGetRoomList p) const
{
	return  msgpack::pack(p);
}
std::vector<uint8_t> DeserializePacketWriter::packPacket(PTRoomList p) const
{
	return  msgpack::pack(p);
}