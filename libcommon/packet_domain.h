#pragma once
#include <string>

#include <mutex>

struct Room
{
	Room() = default;
	Room(const Room& other) : id(other.id), name(other.name){}
	Room(uint32_t id_room, const std::string& name_room) : id(id_room), name(name_room) {}
	uint32_t id;
	std::string name;

	template<class T>
	void pack(T& packer) {
		packer(id, name);
	}
};











