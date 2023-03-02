#pragma once
#include "cell.h"

class Grass final : public Cell {
public:
	Grass() { setCellType(TypeCell::GRASS); }

	void update(std::map<int, std::shared_ptr<Cell>>& cells) override { }

	bool isReadyUpdate() override { return false; }
};

