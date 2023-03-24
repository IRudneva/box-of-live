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

	PacketType type = PacketType::NO_TYPE;
	virtual void pack(msgpack::Packer& packer) = 0;
	virtual void pack(msgpack::Unpacker& unpacker) = 0;
};

struct PacketWithIdChannel
{
	std::shared_ptr<Packet> packet = nullptr;
	uint32_t id_channel;
};

