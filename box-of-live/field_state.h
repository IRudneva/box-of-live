#pragma once
#include "cell.h"

class FieldState
{
public:
	FieldState() = default;
	
	void addColonyBacterium(unsigned int max_count);

	void addGrass(unsigned int amount_grass);

	void update();

	const std::map<int, std::shared_ptr<Cell>>& getData() const { return cells_; }

	void restart();

private:
//	std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> data_cell_;
	std::map<int, std::shared_ptr<Cell>> cells_;
	Timer timer_grass_ = { 5 };
	Timer tb = { 2 };

	Position getRandomEmptyPosition() const;

	Position getRandomEmptyAdjacent(Position position) const;
};

