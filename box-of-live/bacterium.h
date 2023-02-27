#pragma once
#include <memory>
#include <TGUI/TGUI.hpp>
#include "cell.h"

class Bacterium: public Cell {
public:
	Bacterium(unsigned int id_type)
		:id_type_(id_type)
	{
		setCellType(TypeCell::BACTERIUM);
	}

	//void setPosition(Position pos) { position_ = pos; }

	//Position getPosition() { return  position_; }
	void setIdType(unsigned int id) { id_type_ = id; }

	const unsigned int getIdType() { return id_type_; }

	void setEnergy(unsigned int energy) { energy_base_ = energy; }

	unsigned int getEnergy() { return energy_base_; }

	void setSpeed(unsigned int speed) { speed_ = speed; }

	unsigned int getSpeed() const { return speed_; }

//	std::chrono::time_point<std::chrono::steady_clock> getLastActionTime() { return action_time_; }

	bool canMove()
	{
		const auto old_energy = energy_base_;
		update();
		const auto new_energy = energy_base_;
		if (old_energy > new_energy)
		{
			return true;
		}
		return false;
	}
	
	void update() override
	{
		if (energy_base_ != 0)
		{
			--energy_base_;
		//	action_time_ = std::chrono::steady_clock::now();
		}
	}

private:
	unsigned int id_type_ = 0;
	//Position position_;
	unsigned int energy_base_ = 0;
	unsigned int speed_ = 0;
	//std::chrono::time_point<std::chrono::steady_clock> action_time_ = std::chrono::steady_clock::now();
	/*Timer timer_ = { 2 };*/
//	std::chrono::seconds speed_ = std::chrono::seconds(0);
};




