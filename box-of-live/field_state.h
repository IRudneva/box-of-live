#pragma once
#include "cell.h"

class FieldState
{
public:
	FieldState() = default;
	
	void addColonyBacterium(unsigned int max_count);

	void addGrass(unsigned int amount_grass);

	void update();

	const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& getData() const { return data_cell_; }

	void restart();

private:
	std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> data_cell_;
	std::map<int, std::shared_ptr<Cell>> cells_;
	//std::unordered_set<std::shared_ptr<Cell>> data_bacterium_; // ץור?
	//GameConfig game_config_;
	Timer timer_grass_ = { 5 };
	//size_t auto_id_ = 0;
	//Timer timer_bacterium_ = { 2 };

	Position getRandomEmptyPosition() const;

	Position getRandomEmptyAdjacent(Position position) const;
};

