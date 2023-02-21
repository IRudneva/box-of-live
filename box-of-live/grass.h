#pragma once
#include "cell.h"

class Grass final : public Cell {
public:
	Grass() { setColor(tgui::Color::Green); }

	void update() override {}

	~Grass() = default;
};

