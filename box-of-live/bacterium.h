#pragma once
#include <memory>
#include <optional>
#include <TGUI/TGUI.hpp>
#include "cell.h"

constexpr static int ENERGY_ACTION_COST = 1;
constexpr static int ENERGY_BASE = 5;
constexpr static int ENERGY_TO_CLONE = 8;
constexpr static time_t UPDATE_TIME = 2;
constexpr static int ENERGY_GRASS = 3;


class Bacterium: public Cell {
public:
	Bacterium(unsigned int id_type);
	
	bool isReadyUpdate() override;

	void setIdType(unsigned int id) { id_type_ = id; }

	unsigned int getIdType() const { return id_type_; }

	void setEnergy(unsigned int energy) { energy_base_ = energy; }

	unsigned int getEnergy() const { return energy_base_; }

	void setSpeed(unsigned int speed) { speed_ = speed; }

	unsigned int getSpeed() const { return speed_; }
	   	
	void update(const std::map<int, std::shared_ptr<Cell>>& cells) override;

private:
	unsigned int id_type_ = 0;
	double energy_base_ = ENERGY_BASE;
	unsigned int speed_ = UPDATE_TIME;
	time_t last_action_time_;


	bool canMove() const;
	
	bool canClone() { return energy_base_ >= ENERGY_TO_CLONE; }

	void spendEnergy(unsigned int count);

	void increaseEnergy(unsigned int count) { energy_base_ += count; }

	std::shared_ptr<Cell> clone(const std::map<int, std::shared_ptr<Cell>>& cells);

	bool tryMovePriorityCell(const std::map<int, std::shared_ptr<Cell>>& cells);

	int tryFindBacteriumAnotherType(const std::map<int, std::shared_ptr<Cell>>& cells);

	int tryFindGrass(const std::map<int, std::shared_ptr<Cell>>& cells);

	Position tryMoveEmptyCell(const std::map<int, std::shared_ptr<Cell>>& cells);
};




