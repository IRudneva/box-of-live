#pragma once
#include <string>

#include <mutex>

struct IdRoom
{
	static IdRoom* instance() {
		static IdRoom inst;
		return &inst;
	}

	uint32_t generateId() {
		std::lock_guard<std::mutex> lock(m);
		++id;
		return id;
	}

	void reset() { id = 0; }
private:
	IdRoom() = default;
	std::mutex m;
	inline static uint32_t id = 0;
};

struct Room
{
	Room() : id_room(IdRoom::instance()->generateId()){}
	Room(const std::string& n) : id_room(IdRoom::instance()->generateId()), name(n) {}

	uint32_t getId() const { return id_room; }

	template<class T>
	void pack(T& packer) {
		packer(id_room, name);
	}

	std::string name;
private:
	uint32_t id_room;
};











