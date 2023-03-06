#pragma once
#include <memory>
#include <TGUI/TGUI.hpp>
#include "cell.h"

using Millisec = std::chrono::milliseconds;
using TimePoint = std::chrono::steady_clock::time_point;
using AdjacentCellsUMap = std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>;

constexpr static int ENERGY_ACTION_COST = 1;
constexpr static int ENERGY_BASE = 5;
constexpr static int ENERGY_TO_CLONE = 8;
constexpr static Millisec UPDATE_TIME = Millisec(2000);
constexpr static int ENERGY_GRASS = 3;


class Bacterium: public Cell {
public:
	explicit Bacterium(int id_type) :id_type_(id_type) { setCellType(TypeCell::BACTERIUM); }
	
	void update(FieldState& cells) override;

public:
	void setIdType(int id) { id_type_ = id; }

	void setEnergy(int energy) { energy_base_ = energy; }

	//void setSpeed(millisec speed) { speed_ = speed; }

	int getIdType() const { return id_type_; }

	int getEnergy() const { return energy_base_; }
	
	//millisec getSpeed() const { return speed_; }

private:
	int id_type_ = 0;
	int energy_base_ = ENERGY_BASE;
	//millisec speed_ = UPDATE_TIME;
	TimePoint last_action_time_;

private:
	bool isReadyUpdate();

	bool canClone() const { return energy_base_ >= ENERGY_TO_CLONE; }
	
	bool tryEatAnotherBacterium(std::shared_ptr<Cell> another_bacterium);

	bool checkAllBacteriumAroundSameType(const AdjacentCellsUMap& adj_cells) const;
	
	void eatGrass(std::shared_ptr<Cell> grass);

	void spendEnergy(int count);

	void increaseEnergy(int count) { energy_base_ += count; }

	void changeDirection(FieldState& field_state);

	int findPriorytyCell(const AdjacentCellsUMap& adj_cells, TypeCell type) const;

	std::shared_ptr<Bacterium> clone(const AdjacentCellsUMap& adj_cells);

	Position findEmptyCell(const AdjacentCellsUMap& adj_cells) const;
};




