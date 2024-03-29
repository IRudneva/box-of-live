#pragma once
#include <random>

constexpr static int unsigned HEIGHT_WINDOW = 600;
constexpr static unsigned int WIDTH_WINDOW = 800;
constexpr static int HEIGHT_PLAYING_FIELD = static_cast<int>(HEIGHT_WINDOW * 0.75);
constexpr static int WIDTH_PLAYING_FIELD = static_cast<int>(WIDTH_WINDOW * 0.75);
constexpr static int CELL_SIZE = 8;
constexpr static int MAX_COUNT_POSITION_X = WIDTH_PLAYING_FIELD / CELL_SIZE;
constexpr static int MAX_COUNT_POSITION_Y = HEIGHT_PLAYING_FIELD / CELL_SIZE;
constexpr static int NO_RESULT = -1;

enum class TypeCell : uint32_t
{
	BACTERIUM = 1,
	GRASS = 2,

	EMPTY = 0
};

struct SrvColor
{
	SrvColor() = default;
	SrvColor(int r, int g, int b):
	red(r), green(g), blue(b) {}
	int red = 0;
	int green = 0;
	int blue = 0;

	virtual void pack(msgpack::Packer& packer) const { packer(red, green, blue); }
	virtual void pack(msgpack::Unpacker& unpacker) { unpacker(red, green, blue); }
};

static int getRandomInt(int from, int to)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> gen(from, to); 

	return gen(rng);
}

struct GrassInfo {
	GrassInfo() = default;
	GrassInfo(int ox, int oy, bool is_super) : x(ox), y(oy), is_super_grass(is_super) {}
	int x = 0;
	int y = 0;
	bool is_super_grass = false;

	virtual void pack(msgpack::Packer& packer) const { packer(x, y, is_super_grass); }
	virtual void pack(msgpack::Unpacker& unpacker) { unpacker(x, y, is_super_grass); }
}; 

struct BacteriumInfo 
{
	BacteriumInfo() = default;
	BacteriumInfo(int ox, int oy, int id, int en) : x(ox), y(oy), id_type(id), energy(en) {}
	int x = 0;
	int y = 0;
	int id_type = 0;
	int energy = 0;

	void pack(msgpack::Packer& packer) const { packer(x, y, id_type, energy); }
	void pack(msgpack::Unpacker& unpacker) { unpacker(x, y,id_type, energy); }
};

struct DeletedPosition
{
	DeletedPosition() = default;
	DeletedPosition(int ox, int oy) : x(ox), y(oy){}
	int x = 0;
	int y = 0;
	void pack(msgpack::Packer& packer) const { packer(x, y); }
	void pack(msgpack::Unpacker& unpacker) { unpacker(x, y); }
};

struct GameConfig
{
	GameConfig() = default;
	GameConfig(int eb, int eac, int etc, int cc, int cb, int miut, int maut, int gut, int cgr, int efg, int dgfs)
		:energy_base(eb),energy_action_cost(eac), energy_to_clone(etc), count_colonies(cc), count_bacterium(cb),
	min_update_time(miut), max_update_time(maut), grass_update_time(gut), count_grass(cgr),
	energy_from_grass(efg), delta_game_field_size(dgfs){}

	int energy_base = 0;
	int energy_action_cost = 0;
	int energy_to_clone = 0;
	int count_colonies = 0;
	int count_bacterium = 0;
	int min_update_time = 0;
	int max_update_time = 0;
	int grass_update_time = 0;
	int count_grass = 0;
	int energy_from_grass = 0;
	int delta_game_field_size = 10;

	void pack(msgpack::Packer& packer) const { packer(energy_base, energy_action_cost, energy_to_clone, count_colonies, count_bacterium,
		min_update_time, max_update_time, grass_update_time,
		count_grass, energy_from_grass, delta_game_field_size); }

	void pack(msgpack::Unpacker& unpacker) { unpacker(energy_base, energy_action_cost, energy_to_clone, count_colonies, count_bacterium,
		min_update_time, max_update_time, grass_update_time,
		count_grass, energy_from_grass, delta_game_field_size); }

};

struct DbRoomInfo
{
	DbRoomInfo() = default;
	DbRoomInfo(bool status, GameConfig conf) : is_active(status), config(conf) {}
	bool is_active = false;
	GameConfig config;
};