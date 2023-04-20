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
	double delta_ = 1.0;

	Position operator+(const PositionDelta& delta) const { return Position{ x + delta.x, y + delta.y }; }

	Position operator*(double delta) const { return Position{ static_cast<int>(x * delta), static_cast<int>(y * delta) }; }

	const Position getRandomDirection() const;

	const std::vector<Position> getAllAdjacentPosition() const;

	bool operator== (const Position& other) const { return x == other.x && y == other.y && delta_ == other.delta_; }
};

inline bool operator!= (const Position& lhs, const Position& rhs) { return !(rhs == lhs); }

struct PositionHasher
{
	size_t operator() (const Position& p) const;
private:
	std::hash<int> ui_hasher_;
};

static Position getRandomPosition(double delta)
{
	int rand_x = getRandomInt(0, static_cast<int>(MAX_COUNT_POSITION_X * delta));
	int rand_y = getRandomInt(0, static_cast<int>(MAX_COUNT_POSITION_Y * delta));
	return { rand_x, rand_y };
}
