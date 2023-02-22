#pragma once
#include "cell.h"

class FieldState
{
public:
	FieldState() = default;

	void addBacterium(unsigned int amount_species, unsigned int max_count);

	void addGrass(unsigned int amount_grass);

	void update();

	const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& getData() const { return data_cell_; }

private:
	std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> data_cell_;
};

