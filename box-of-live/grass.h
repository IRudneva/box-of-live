#pragma once
#include "cell.h"

class Grass final : public Cell {
public:
	Grass() { setCellType(TypeCell::GRASS); }

	void update(FieldState& cells) override { }
};

