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

	TypeCell getCellType() const { return type_; }

	void setCellType(TypeCell type) { type_ = type; }

private:
	TypeCell type_ = TypeCell::EMPTY;
};


class EmptyCell final :public Cell
{
public:
	EmptyCell() { setCellType(TypeCell::EMPTY); }
	void update() override {}
};
