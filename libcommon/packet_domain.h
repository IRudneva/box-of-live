#pragma once
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











