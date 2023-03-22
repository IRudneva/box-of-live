#pragma once
#include <vector>
#include <memory>

enum class PacketType : uint32_t
{
	PT_CREATE_ROOM,
	PT_CLOSE_ROOM,
	PT_GET_ROOM_LIST,
	PT_ROOM_LIST
};

struct PacketHeader
{
	PacketType  packet_type;
	uint32_t data_size = 0;
};

struct DeserializePacket
{
protected:
	DeserializePacket(PacketType ty) : type(ty) {}
public:
	PacketType type;
	virtual ~DeserializePacket() = default;
};

struct NetworkPacket
{
	PacketHeader header;
	std::vector<uint8_t> data;

	template<class T>
	void pack(T& packer) {
		packer(data);
	}
};

struct SerializedPacketWithIdChannel
{
	std::shared_ptr<NetworkPacket> packet = nullptr;
	uint32_t id_channel;
};

struct DeserializePacketWithIdChannel
{
	std::shared_ptr<DeserializePacket> packet = nullptr;
	uint32_t id_channel;
};

