#pragma once
#include "domain.h"
#include <TGUI/TGUI.hpp>

class Cell
{
protected:
	Cell() = default;

public:
	virtual ~Cell() = default;

	virtual void update(std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> data_cell) = 0;

	TypeCell getCellType() const { return type_; }

	void setCellType(TypeCell type) { type_ = type; }

private:
	TypeCell type_ = TypeCell::EMPTY;
};
