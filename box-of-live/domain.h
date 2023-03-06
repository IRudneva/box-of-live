#pragma once
#include <functional>
#include <chrono>
#include <map>
#include <random>
#include <memory>
#include <optional>

constexpr static int unsigned HEIGHT_WINDOW = 600;

constexpr static unsigned int WIDTH_WINDOW = 800;

constexpr static int HEIGHT_PLAYING_FIELD = 400;

constexpr static int WIDTH_PLAYING_FIELD = 800;

constexpr static int CELL_SIZE = 8;

constexpr static int NUMBER_BACTERIAL_COLONIES = 3;

constexpr static int COUNT_POSITION_X = WIDTH_PLAYING_FIELD / CELL_SIZE;

constexpr static int COUNT_POSITION_Y = HEIGHT_PLAYING_FIELD / CELL_SIZE;

constexpr  static int NO_RESULT = -1;

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
		std::function<void(int)> setterFunction;
	};

void init(std::shared_ptr<GameConfig> config)
{
	ConfigRecord energy_base_record;
	energy_base_record.setterFunction = [&config](int value) {
		config->energy_base = value;
	};
	energy_base_record.defaultValue = 5;
	records["enegry_base"] = energy_base_record;

	ConfigRecord energy_action_record;
	energy_action_record.setterFunction = [&config](int value) {
		config->energy_action_cost = value;
	};
	energy_action_record.defaultValue = 1;
	records["energy_action_cost"] = energy_action_record;

	ConfigRecord energy_clone_record;
	energy_clone_record.setterFunction = [&config](int value) {
		config->energy_to_clone = value;
	};
	energy_clone_record.defaultValue = 8;
	records["enegry_to_clone"] = energy_clone_record;

	ConfigRecord update_record;
	update_record.setterFunction = [&config](int value) {
		config->update_time = value;
	};
	update_record.defaultValue = 2;
	records["update_time"] = update_record;

	ConfigRecord grass_update_record;
	grass_update_record.setterFunction = [&config](int value) {
		config->grass_update_time = value;
	};
	grass_update_record.defaultValue = 5;
	records["grass_update_time"] = grass_update_record;
}

	void setOption(std::shared_ptr<GameConfig> gameConfig, const std::string & str, int value)
	{
		records[str].setterFunction(value);
	}

	void doWithAll(const std::function<void(const std::string &, int)> & aVisitor)
	{
		for (const auto &[optionName, configRecord] : records) {
			aVisitor(optionName, configRecord.defaultValue);
		}
	}

	const std::map<std::string, ConfigRecord>& getRecords() const { return records; }
	
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
	int x = 0;
	int y = 0;

	Position operator+(const PositionDelta& delta) const
	{
		return Position{ x + delta.x, y + delta.y };
	}

	const Position getRandomDirection() const
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

	const std::vector<Position> getAllAdjacentPosition() const
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
	}
private:
	std::hash<int> ui_hasher_;
};

static Position getRandomPosition()
{
	int rand_x = getRandomInt(0, COUNT_POSITION_X);
	int rand_y = getRandomInt(0, COUNT_POSITION_Y);
	return { rand_x, rand_y };
}

static std::chrono::steady_clock::time_point getCurrentTime() {

	std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();

	return time;
}

class Timer {
public:
	Timer(std::chrono::milliseconds interval) : interval(interval) {
		reset();
	}

	bool timedOut() {
		if (std::chrono::duration_cast<std::chrono::milliseconds>(getCurrentTime() - start)  >= interval) {
			reset();
			return true;
		}
		return false;
	}

	void reset() {
		start = getCurrentTime();
	}

private:
	std::chrono::steady_clock::time_point start;
	const std::chrono::milliseconds interval;
};
