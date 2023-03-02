#pragma once
#include "cell.h"

class FieldState
{
public:
	FieldState() = default;

	void update();

	const std::map<int, std::shared_ptr<Cell>>& getData() const { return cells_; }

	void restart();

private:
	std::map<int, std::shared_ptr<Cell>> cells_;
	Timer timer_grass_ = { 3 };

	Position getRandomEmptyPosition() const;

	Position getRandomEmptyAdjacent(Position position) const;

	void addColonyBacterium(int max_count);

	void addGrass(int amount_grass);
};

