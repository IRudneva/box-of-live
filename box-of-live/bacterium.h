#pragma once
#include <memory>
#include <TGUI/TGUI.hpp>
#include "cell.h"

class Bacterium: public Cell{
public:
	Bacterium() {
		setColor(tgui::Color::Red);
	}

	void update() override {}

	~Bacterium() = default;
};


