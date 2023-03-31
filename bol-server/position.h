#pragma once
#include "game_domain.h"

struct PositionDelta
{
	int x = 0;
	int y = 0;
};

struct Position
{
	int x = 0;
	int y = 0;

	Position operator+(const PositionDelta& delta) const { return Position{ x + delta.x, y + delta.y }; }

	const Position getRandomDirection() const;

	const std::vector<Position> getAllAdjacentPosition() const;

	bool operator== (const Position& other) const { return x == other.x && y == other.y; }
};

inline bool operator!= (const Position& lhs, const Position& rhs) { return !(rhs == lhs); }

struct PositionHasher
{
	size_t operator() (const Position& p) const;
private:
	std::hash<int> ui_hasher_;
};

static Position getRandomPosition()
{
	int rand_x = getRandomInt(0, COUNT_POSITION_X);
	int rand_y = getRandomInt(0, COUNT_POSITION_Y);
	return { rand_x, rand_y };
}
