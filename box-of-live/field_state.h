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

private:
	std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> data_cell_;
	GameConfig game_config_;
	Timer timer_grass_ = { 5 };
	Timer timer_bacterium_ = { 2 };
};

