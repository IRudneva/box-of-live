#pragma once
#include <random>

struct Position
{
	unsigned int x = 0;
	unsigned int y = 0;

	bool operator== (const Position& other) const {return x == other.x && y == other.y; }
};

struct PosHasher {
	size_t operator() (const Position& p) const { return p.x + 100 + p.y < p.x + 100 + p.y; }
};

static bool operator<(const Position& lhs, const Position& rhs) { return lhs.x < rhs.x && lhs.y < rhs.y; }

constexpr static unsigned int HEIGHT_WINDOW = 600;

constexpr static unsigned int WIDTH_WINDOW = 800;

constexpr static unsigned int HEIGHT_PLAYING_FIELD = 400;

constexpr static unsigned int WIDTH_PLAYING_FIELD = 800;

constexpr static unsigned int CELL_SIZE = 8;

inline unsigned int getRandInt(unsigned int from, unsigned int to) { return from + std::rand() % (to - from); }