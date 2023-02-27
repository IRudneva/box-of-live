#pragma once
#include <memory>
#include <TGUI/TGUI.hpp>
#include "cell.h"

constexpr static int ENERGY_ACTION_COST = 1;
constexpr static int ENERGY_BASE = 5;
constexpr static int ENERGY_TO_CLONE = 3;
constexpr static int UPDATE_TIME = 2;

class Bacterium: public Cell {
public:
	Bacterium(unsigned int id_type)
		:id_type_(id_type)
	{
		setCellType(TypeCell::BACTERIUM);
	}

	void setPosition(Position pos) { position_ = pos; }

	Position getPosition() const { return  position_; }
	
	void setIdType(unsigned int id) { id_type_ = id; }

	unsigned int getIdType() const { return id_type_; }

	void setEnergy(unsigned int energy) { energy_base_ = energy; }

	unsigned int getEnergy() const { return energy_base_; }

	void setSpeed(unsigned int speed) { speed_ = speed; }

	unsigned int getSpeed() const { return speed_; }
	   	
	void update(std::unordered_map<Position,std::shared_ptr<Cell>,PositionHasher> data_cell) override
	{
		if (!canMove())
			return;
		/////////////////////////////////////////////////////////////////////////////////////
		//ПЕРЕНЕСТИ ВСЮ ЛОГИКУ ОБНОВЛЕНИЯ КЛЕТКИ БАКТЕРИИ ИЗ СОСТОЯНИЯ ПОЛЯ
	}

private:
	unsigned int id_type_ = 0;
	Position position_;
	unsigned int energy_base_ = ENERGY_BASE;
	unsigned int speed_ = UPDATE_TIME;
	std::chrono::time_point<std::chrono::steady_clock> last_action_time_;


	bool canMove() const
	{
		if (energy_base_ > ENERGY_ACTION_COST)
		{
			return true;
		}
		return false;
	}
};




