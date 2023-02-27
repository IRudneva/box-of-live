#pragma once
#include <functional>
#include <chrono>
#include <map>
#include <random>

constexpr static unsigned int HEIGHT_WINDOW = 600;

constexpr static unsigned int WIDTH_WINDOW = 800;

constexpr static unsigned int HEIGHT_PLAYING_FIELD = 400;

constexpr static unsigned int WIDTH_PLAYING_FIELD = 800;

constexpr static unsigned int CELL_SIZE = 8;

constexpr static unsigned int NUMBER_BACTERIAL_COLONIES = 3;

//epBase - базовое количество энергии у бактерии
//epActionCost - количество энергии потраченное на действие
//epToClone - количество энергии необходимое для деления
//updateTime - время как часто бактерия делает действие
//grassUpdateTime - время как часто появляется новая трава

struct GameConfig
{
	int energy_base = 0;
	int energy_action_cost = 0;
	int energy_to_clone = 0;
	int update_time = 0;
	int grass_update_time = 0;
};

struct ConfigHelper
{
	struct ConfigRecord
	{
		int defaultValue;
		std::function<void(GameConfig&, int)> setterFunction;
	};

void init()
{
	ConfigRecord energy_base_record;
	energy_base_record.setterFunction = [](GameConfig& config, int value) {
		config.energy_base = value;
	};
	energy_base_record.defaultValue = 5;
	records["enegry_base"] = energy_base_record;

	ConfigRecord energy_action_record;
	energy_action_record.setterFunction = [](GameConfig& config, int value) {
		config.energy_action_cost = value;
	};
	energy_action_record.defaultValue = 1;
	records["energy_action_cost"] = energy_action_record;

	ConfigRecord energy_clone_record;
	energy_clone_record.setterFunction = [](GameConfig& config, int value) {
		config.energy_to_clone = value;
	};
	energy_clone_record.defaultValue = 3;
	records["enegry_to_clone"] = energy_clone_record;

	ConfigRecord update_record;
	update_record.setterFunction = [](GameConfig& config, int value) {
		config.update_time = value;
	};
	update_record.defaultValue = 2;
	records["update_time"] = update_record;

	ConfigRecord grass_update_record;
	grass_update_record.setterFunction = [](GameConfig& config, int value) {
		config.grass_update_time = value;
	};
	grass_update_record.defaultValue = 5;
	records["grass_update_time"] = grass_update_record;
}

	void SetOption(GameConfig & gameConfig, const std::string & str, int value)
	{
		records[str].setterFunction(gameConfig, value);
	}

	void DoWithAll(const std::function<void(const std::string &, int)> & aVisitor)
	{
		for (const auto &[optionName, configRecord] : records) {
			aVisitor(optionName, configRecord.defaultValue);
		}
	}

	std::map<std::string, ConfigRecord> records;
};

enum class TypeCell
{
	BACTERIUM,
	GRASS,

	EMPTY
};

inline unsigned int getRandomUInt(unsigned int from, unsigned int to) { return from + std::rand() % (to - from); }

struct Position
{
	unsigned int x = 0;
	unsigned int y = 0;

	const Position& getRandomDirection() const
	{
		std::vector<Position> neighbour_pos = { 
		{x - CELL_SIZE, y + CELL_SIZE},
		{x, y + CELL_SIZE},
		{x + CELL_SIZE, y + CELL_SIZE},
		{ x + CELL_SIZE, y},
		{ x + CELL_SIZE, y - CELL_SIZE},
		{ x, y - CELL_SIZE},
		{ x - CELL_SIZE, y - CELL_SIZE} ,
		{ x - CELL_SIZE, y}
		};
		
		unsigned int rand_neig = getRandomUInt(0, neighbour_pos.size() - 1);

		auto neigh_pos = neighbour_pos[rand_neig];

		return neigh_pos;
	}

	bool operator== (const Position& other) const { return x == other.x && y == other.y; }
};

struct PositionHasher
{
	size_t operator() (const Position& p) const { return p.x + 100 + p.y < p.x + 100 + p.y; }
};

//static bool operator<(const Position& lhs, const Position& rhs) { return lhs.x < rhs.x && lhs.y < rhs.y; }


inline Position getRandomPosition()
{
	unsigned int rand_x = CELL_SIZE * getRandomUInt(0, WIDTH_PLAYING_FIELD / CELL_SIZE);
	unsigned int rand_y = CELL_SIZE * getRandomUInt(0, HEIGHT_PLAYING_FIELD / CELL_SIZE);
	return { rand_x, rand_y };
}

static time_t getCurrentTime() {

	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();

	return  std::chrono::system_clock::to_time_t(p);
}

class Timer {
public:
	Timer(time_t interval) : interval(interval) {
		reset();
	}

	bool timedOut() {
		if (getCurrentTime() >= deadline) {
			reset();
			return true;
		}
		else return false;
	}

	void reset() {
		deadline = getCurrentTime() + interval;
	}

private:
	time_t deadline;
	const time_t interval;
};