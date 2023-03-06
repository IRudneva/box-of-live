#pragma once
#include "domain.h"
#include "field_state.h"

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

class FieldState;

class Cell
{
protected:
	Cell() { id_ = IdCell::generateID(); }
	Position position_;

public:
	virtual ~Cell() = default;

	virtual void update(FieldState& cells) = 0;

	TypeCell getCellType() const { return type_; }

	void setCellType(TypeCell type) { type_ = type; }

	void setPosition(Position pos)
	{
		if (pos != position_)
			position_ = pos;
	}

	Position getPosition() const { return  position_; }

	int getIdCell() const { return id_; }

private:
	int id_ = 0;
	TypeCell type_ = TypeCell::EMPTY;
};
