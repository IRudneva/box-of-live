#pragma once
#include <optional>
#include <random>

constexpr static int unsigned HEIGHT_WINDOW = 600;
constexpr static unsigned int WIDTH_WINDOW = 800;
constexpr static int HEIGHT_PLAYING_FIELD = 400;
constexpr static int WIDTH_PLAYING_FIELD = 800;
constexpr static int CELL_SIZE = 8;
constexpr static int NUMBER_BACTERIAL_COLONIES = 3;
constexpr static int COUNT_POSITION_X = WIDTH_PLAYING_FIELD / CELL_SIZE;
constexpr static int COUNT_POSITION_Y = HEIGHT_PLAYING_FIELD / CELL_SIZE;
constexpr static int NO_RESULT = -1;

enum class TypeCell
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

struct BacteriumInfo
{
	BacteriumInfo() = default;
	BacteriumInfo(int id, int en):id_type(id),energy(en) {}
	int id_type = 0;
	int energy = 0;
	template<class T>
	void pack(T& packer) {
		packer(id_type, energy);
	}
};

struct CellInfo {
	CellInfo() = default;
	CellInfo(TypeCell t, int ox, int oy, std::shared_ptr<BacteriumInfo> inf) : type(t),x(ox), y(oy), bacterium_info(inf) {}
	TypeCell type = TypeCell::EMPTY;
	int x = 0;
	int y = 0;
	std::shared_ptr<BacteriumInfo> bacterium_info = nullptr;

	template<class T>
	void pack(T& packer) {
		packer(x, y, *bacterium_info);
	}
};

struct GameConfig
{
	int energy_base = 0;
	int energy_action_cost = 0;
	int energy_to_clone = 0;
	int update_time = 0;
	int grass_update_time = 0;
	int energy_from_grass = 0;

	template<class T>
	void pack(T& packer) {
		packer(energy_base, energy_action_cost, energy_to_clone, update_time, grass_update_time, energy_from_grass);
	}
};