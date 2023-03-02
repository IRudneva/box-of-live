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
	std::optional<int> erase_id_ = std::nullopt;

public:
	virtual ~Cell() = default;

	virtual void update(const std::map<int, std::shared_ptr<Cell>>& cells) = 0;

	virtual bool isReadyUpdate() = 0;

	virtual std::optional<int> getEraseId() { return erase_id_; }

	virtual bool canClone() = 0;

	virtual std::shared_ptr<Cell> clone(const std::map<int, std::shared_ptr<Cell>>& cells) = 0;

	TypeCell getCellType() const { return type_; }

	void setCellType(TypeCell type) { type_ = type; }

	void setPosition(Position pos) { position_ = pos; }

	Position getPosition() const { return  position_; }

	int getIdCell() const { return id_; }


private:
	int id_ = 0;
	TypeCell type_ = TypeCell::EMPTY;
};
