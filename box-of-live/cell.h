#pragma once
#include <optional>
#include "domain.h"
#include <TGUI/TGUI.hpp>

struct IdCell
{
	static int generateID(){
		id++;
		return id;
	}
	static void reset() { id = 0; }
private:
	inline static int id = 0;
};

class Cell
{
protected:
	Cell()
	{
		id_ = IdCell::generateID();
	}

	Position position_;

public:
	virtual ~Cell() = default;

	virtual UpdateState update(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell) = 0;

	TypeCell getCellType() const { return type_; }

	void setCellType(TypeCell type) { type_ = type; }

	void setPosition(Position pos) { position_ = pos; }

	Position getPosition() const { return  position_; }

	int getIdCell() const { return id_; }

	//void setIdCell(int id) { id_cell_ = id; }

private:
	int id_ = 0;
	TypeCell type_ = TypeCell::EMPTY;
};
