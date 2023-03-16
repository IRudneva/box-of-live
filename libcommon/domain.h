#pragma once
#include <chrono>
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

static std::chrono::steady_clock::time_point getCurrentTime() {
	std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
	return time;
}

struct BacteriumInfo
{
	int id_type = 0;
	int energy = 0;
};

struct CellInfo {
	TypeCell type = TypeCell::EMPTY;
	int x = 0;
	int y = 0;
	std::optional<BacteriumInfo> bacterium_info;
};

enum class UIEventType
{
	PRESSED_BUTTON_CREATE_ROOM,
	PRESSED_BUTTON_CHOOSE_ROOM,

	NO_EVENT
};

class Timer {
public:
	void initInt(int interval)
	{
		interval_ = std::chrono::milliseconds(std::chrono::seconds(interval));
		reset();
	}

	void initDouble(double interval)
	{
		int seconds = (int)interval;
		int milli = (int)((interval - seconds) * 1000);
		interval_ = std::chrono::milliseconds(milli);
		reset();
	}

	bool timedOut()
	{
		if (std::chrono::duration_cast<std::chrono::milliseconds>(getCurrentTime() - start_) >= interval_) {
			reset();
			return true;
		}
		return false;
	}

	void reset() { start_ = getCurrentTime(); }

private:
	std::chrono::steady_clock::time_point start_;
	std::chrono::milliseconds interval_ = std::chrono::milliseconds(0);
};
