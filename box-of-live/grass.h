#pragma once
#include "cell.h"

class Grass final : public Cell {
public:
	Grass() { setCellType(TypeCell::GRASS); }

	void update(const std::map<int, std::shared_ptr<Cell>>& cells) override { }

	bool isReadyUpdate() override { return false; }

	bool canClone() override { return false; }

	std::shared_ptr<Cell> clone(const std::map<int, std::shared_ptr<Cell>>& cells) override { return nullptr; }
};

