#pragma once
#include <random>

constexpr static unsigned int HEIGHT_WINDOW = 600;

constexpr static unsigned int WIDTH_WINDOW = 800;

constexpr static unsigned int HEIGHT_PLAYING_FIELD = 400;

constexpr static unsigned int WIDTH_PLAYING_FIELD = 800;

constexpr static unsigned int CELL_SIZE = 8;

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