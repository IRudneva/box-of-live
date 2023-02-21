#pragma once
#include "domain.h"
#include <TGUI/TGUI.hpp>

class Cell
{
protected:
	Cell() = default;

public:
	virtual ~Cell() = default;

	virtual void update() = 0;

	const Position& getPosition() const { return position_; }

	const tgui::Layout2d& getSize() const { return size_; }

	const tgui::Color getColor() const { return color_; }

	void setColor(const tgui::Color& color) { color_ = color; }

	void setPosition(const Position& new_pos) { position_ = new_pos; }

private:
	tgui::Color color_ = tgui::Color::White;
	tgui::Layout2d size_ = { 8.0f, 8.0f };
	Position position_;
};
