#pragma once
#include <memory>
#include "packet_type.h"

struct Packet
{
protected:
	Packet() = default;
	explicit Packet(PacketType ty) : type(ty){}
public:
	virtual ~Packet() = default;

	PacketType type;
	virtual void pack(msgpack::Packer& packer) const = 0;
	virtual void pack(msgpack::Unpacker& unpacker) = 0;
};

struct ConnectionMessage : Packet
{
	ConnectionMessage(PacketType ty) :Packet(ty) {}

	void pack(msgpack::Packer& packer) const  override {}
	void pack(msgpack::Unpacker& unpacker) override {}
};

struct PacketHeader
{
	PacketType  packet_type;
	uint32_t data_size = 0;
};

