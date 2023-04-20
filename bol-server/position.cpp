#include "pch_server.h"
#include "position.h"

const Position Position::getRandomDirection() const
{
	Position curr_pos = { x,y };

	int minXDelta = x > 0 ? -1 : 0;
	int maxXDelta = x < static_cast<int>(MAX_COUNT_POSITION_X * delta_) ? 1 : 0;

	int minYDelta = y > 0 ? -1 : 0;
	int maxYDelta = y < static_cast<int>(MAX_COUNT_POSITION_Y* delta_) ? 1 : 0;

	auto positionDelta =
		PositionDelta{
		  getRandomInt(minXDelta, maxXDelta),
		  getRandomInt(minYDelta, maxYDelta)
	};
	return curr_pos + positionDelta;
}

const std::vector<Position> Position::getAllAdjacentPosition() const
{
	Position curr_pos = { x,y };

	std::vector<Position> adjacent_position;
	adjacent_position.reserve(8);

	auto delta_min = PositionDelta{ 0, -1 };
	auto start_pos = curr_pos + PositionDelta{ -2, 1 };
	auto old_start = start_pos;
	int count_row = 1;
	while (count_row < 4)
	{
		int count_col = 1;
		while (count_col < 4)
		{
			auto delta = PositionDelta{ 1, 0 };
			auto pos = start_pos + delta;
			if (!(pos == curr_pos))
			{
				if ((pos.x > 0 && pos.y > 0) && (pos.x < static_cast<int>(MAX_COUNT_POSITION_X * delta_) && pos.y < static_cast<int>(MAX_COUNT_POSITION_Y * delta_)))
				{
					adjacent_position.push_back({ pos.x,pos.y });
				}
			}
			start_pos = pos;
			++count_col;
		}
		start_pos = old_start + delta_min;
		old_start = start_pos;
		++count_row;
	}
	return adjacent_position;
}

size_t PositionHasher::operator() (const Position& p) const
{
	size_t h_x = ui_hasher_(p.x);
	size_t h_y = ui_hasher_(p.y);
	return h_x * 137 + h_y * (137 * 137);
}
