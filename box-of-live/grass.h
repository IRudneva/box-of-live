#pragma once
#include "cell.h"

class Grass final : public Cell {
public:
	Grass() { setCellType(TypeCell::GRASS); }

	UpdateState update(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell) override { return{}; }
};

