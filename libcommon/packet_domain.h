#pragma once
#include <string>
#include "game_domain.h"
#include <mutex>

struct Room
{
	Room() = default;
	Room(const Room& other) : id(other.id), name(other.name), config(std::move(other.config)){}
	Room(uint32_t id_room, const std::string& name_room, std::shared_ptr<GameConfig> conf) : id(id_room), name(name_room), config(std::move(conf)) {}
	uint32_t id;
	std::string name;
	std::shared_ptr<GameConfig> config = std::make_shared<GameConfig>();

	template<class T>
	void pack(T& packer) {
		packer(id, name, *config);
	}
};











