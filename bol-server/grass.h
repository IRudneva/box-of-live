#pragma once
#include "cell.h"

class Grass final : public Cell {
public:
	Grass() { setCellType(TypeCell::GRASS); }

	Grass(bool is_super)
	{
		setCellType(TypeCell::GRASS);
		is_super_grass_ = is_super;
	}

	void update(FieldState& cells) override { }

	bool isSuperGrass() const { return is_super_grass_; }

	void onEaten() const {}
private:
	bool is_super_grass_ = false;
};

