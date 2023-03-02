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

constexpr static unsigned int COUNT_POSITION_X = WIDTH_PLAYING_FIELD / CELL_SIZE;

constexpr static unsigned int COUNT_POSITION_Y = HEIGHT_PLAYING_FIELD / CELL_SIZE;

constexpr  static int NO_RESULT = -1;

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

static int getRandomInt(int from, int to)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> gen(from, to); // uniform, unbiased

	return gen(rng);
}


struct PositionDelta
{
	int x = 0;
	int y = 0;
};

struct Position
{
	unsigned int x = 0;
	unsigned int y = 0;

	Position operator+(const PositionDelta& delta) const
	{
		return Position{ x + delta.x, y + delta.y };
	}

	const Position& getRandomDirection() const
	{
		Position curr_pos = { x,y };

		int minXDelta = x > 0 ? -1 : 0;
		int maxXDelta = x < WIDTH_PLAYING_FIELD ? 1 : 0;

		int minYDelta = y > 0 ? -1 : 0;
		int maxYDelta = y < HEIGHT_PLAYING_FIELD ? 1 : 0;

		auto positionDelta =
			PositionDelta{
			  getRandomInt(minXDelta, maxXDelta),
			  getRandomInt(minYDelta, maxYDelta)
		};
		return curr_pos + positionDelta;
	}
	

	const std::vector<Position> getAllAdjacentPosition()
	{
		Position curr_pos = { x,y };

		std::vector<Position> adjacent_position = {
			curr_pos + PositionDelta{-1,1},
			curr_pos + PositionDelta{0, 1},
			curr_pos + PositionDelta{1,1},
			curr_pos + PositionDelta{1, 0},
			curr_pos + PositionDelta{1, -1},
			curr_pos + PositionDelta{0, -1},
			curr_pos + PositionDelta{-1,-1},
			curr_pos + PositionDelta{-1,0}
		};

		return adjacent_position;
	}

	int getCountAdjacent() { return getAllAdjacentPosition().size(); }

	bool operator== (const Position& other) const { return x == other.x && y == other.y; }
};

inline bool operator!= (const Position& lhs, const Position& rhs) { return !(rhs == lhs); }

struct PositionHasher
{
	size_t operator() (const Position& p) const
	{
		size_t h_x = ui_hasher_(p.x);
		size_t h_y = ui_hasher_(p.y);
		return h_x * 137 + h_y * (137 * 137);
		//return p.x + p.y * 10000;
	}
private:
	std::hash<unsigned int> ui_hasher_;
};

inline Position getRandomPosition()
{
	unsigned int rand_x = getRandomInt(0, COUNT_POSITION_X);
	unsigned int rand_y = getRandomInt(0, COUNT_POSITION_Y);
	return { rand_x, rand_y };
}

struct UpdateState
{
	Position old_position;
	Position new_position;

	std::optional<Position> chaild_position;
};

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