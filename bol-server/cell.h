#pragma once
#include "position.h"

struct IdCell
{
	static int generateId() {
		++id;
		return id;
	}
	static void reset() { id = 0; }
private:
	IdCell() = default;
	inline static int id = 0;
};

class FieldState;

class Cell
{
protected:
	Cell() { id_ = IdCell::generateId(); }
	Position position_;

public:
	virtual ~Cell() = default;

	virtual void update(FieldState& cells) = 0;

	void setCellType(TypeCell type) { type_ = type; }

	bool isReadyToReset() const { return type_ == TypeCell::EMPTY; }

	void setPosition(const Position& pos)
	{
		if (pos != position_)
			position_ = pos;
	}

	const Position& getPosition() const { return  position_; }

	int getIdCell() const { return id_; }

	TypeCell getCellType() const { return type_; }

private:
	int id_ = 0;
	TypeCell type_ = TypeCell::EMPTY;
};

