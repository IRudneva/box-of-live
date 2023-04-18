#pragma once
#include <optional>
#include <random>

constexpr static int unsigned HEIGHT_WINDOW = 600;
constexpr static unsigned int WIDTH_WINDOW = 800;
constexpr static int HEIGHT_PLAYING_FIELD = static_cast<int>(HEIGHT_WINDOW * 0.75);
constexpr static int WIDTH_PLAYING_FIELD = static_cast<int>(WIDTH_WINDOW * 0.75);
constexpr static int CELL_SIZE = 8;
constexpr static int NUMBER_BACTERIAL_COLONIES = 3;
constexpr static int COUNT_POSITION_X = WIDTH_PLAYING_FIELD / CELL_SIZE;
constexpr static int COUNT_POSITION_Y = HEIGHT_PLAYING_FIELD / CELL_SIZE;
constexpr static int NO_RESULT = -1;

enum class TypeCell : uint32_t
{
	BACTERIUM,
	GRASS,

	EMPTY
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
	GrassInfo(int ox, int oy) : x(ox), y(oy) {}
	int x = 0;
	int y = 0;
	virtual ~GrassInfo() = default;

	virtual void pack(msgpack::Packer& packer) const { packer(x, y); }
	virtual void pack(msgpack::Unpacker& unpacker) { unpacker(x, y); }
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

	int energy_base = 0;
	int energy_action_cost = 0;
	int energy_to_clone = 0;
	int min_update_time = 0;
	int max_update_time = 0;
	int grass_update_time = 0;
	int count_grass = 0;
	int energy_from_grass = 0;
	double delta_game_field_size = 1.0;

	template<class T>
	void pack(T& packer) {
		packer(energy_base, energy_action_cost, energy_to_clone, 
			min_update_time, max_update_time, grass_update_time, 
			count_grass, energy_from_grass, delta_game_field_size);
	}
};