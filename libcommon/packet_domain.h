#pragma once
#include "deserialize_packet.h"
#include <string>

struct IdRoom
{
	static uint32_t generateId() {
		++id;
		return id;
	}
	static void reset() { id = 0; }
private:
	IdRoom() = default;
	inline static uint32_t id = 0;
};

struct Room
{
	Room() : id_room(IdRoom::generateId()){}
	Room(const std::string& n) : id_room(IdRoom::generateId()), name(n) {}

	uint32_t getId() const { return id_room; }

	template<class T>
	void pack(T& packer) {
		packer(id_room, name);
	}

	std::string name;
private:
	uint32_t id_room;
};

struct PTCreateRoom : public DeserializePacket
{
	PTCreateRoom() : DeserializePacket(PacketType::PT_CREATE_ROOM){}
	PTCreateRoom(const std::string& name_room) : DeserializePacket(PacketType::PT_CREATE_ROOM), room(name_room) {}
	Room room;
	template<class T>
	void pack(T& packer) {
		packer(room);
	}
};

struct PTCloseRoom : public DeserializePacket 
{
	PTCloseRoom() :DeserializePacket(PacketType::PT_CLOSE_ROOM){}
	PTCloseRoom(uint32_t id) : DeserializePacket(PacketType::PT_CLOSE_ROOM), id_room(id) {}
	uint32_t id_room;

	template<class T>
	void pack(T& packer) {
		packer(id_room);
	}
};

struct PTGetRoomList : DeserializePacket
{
	PTGetRoomList() : DeserializePacket(PacketType::PT_GET_ROOM_LIST){}

	uint8_t t = 0;
	template<class T>
	void pack(T& packer) {
		packer(t);
	}
};

struct PTRoomList : DeserializePacket
{
	PTRoomList() : DeserializePacket(PacketType::PT_ROOM_LIST){}

	std::vector<Room> room_list;

	template<class T>
	void pack(T& packer) {
		packer(room_list);
	}
};











