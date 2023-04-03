#pragma once
//#include <chrono>
//#include <optional>
//#include <random>
//
//
//constexpr static int unsigned HEIGHT_WINDOW = 600;
//constexpr static unsigned int WIDTH_WINDOW = 800;
//constexpr static int HEIGHT_PLAYING_FIELD = 400;
//constexpr static int WIDTH_PLAYING_FIELD = 800;
//constexpr static int CELL_SIZE = 8;
//constexpr static int NUMBER_BACTERIAL_COLONIES = 3;
//constexpr static int COUNT_POSITION_X = WIDTH_PLAYING_FIELD / CELL_SIZE;
//constexpr static int COUNT_POSITION_Y = HEIGHT_PLAYING_FIELD / CELL_SIZE;
//constexpr static int NO_RESULT = -1;
//
//enum class TypeCell
//{
//	BACTERIUM,
//	GRASS,
//
//	EMPTY
//};
//
//static int getRandomInt(int from, int to)
//{
//	std::random_device rd;
//	std::mt19937 rng(rd());
//	std::uniform_int_distribution<int> gen(from, to); 
//
//	return gen(rng);
//}
//
//static std::chrono::steady_clock::time_point getCurrentTime() {
//	std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
//	return time;
//}
//
//struct BacteriumInfo
//{
//	int id_type = 0;
//	int energy = 0;
//};
//
//struct CellInfo {
//	TypeCell type = TypeCell::EMPTY;
//	int x = 0;
//	int y = 0;
//	std::optional<BacteriumInfo> bacterium_info;
//};
