#pragma once
#include <optional>
#include "domain.h"
#include <TGUI/TGUI.hpp>

class Cell
{
protected:
	Cell() = default;

	Position position_;

public:
	virtual ~Cell() = default;

	virtual UpdateState update(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell) = 0;

	TypeCell getCellType() const { return type_; }

	void setCellType(TypeCell type) { type_ = type; }

	void setPosition(Position pos) { position_ = pos; }

	Position getPosition() const { return  position_; }

private:
	TypeCell type_ = TypeCell::EMPTY;
};
