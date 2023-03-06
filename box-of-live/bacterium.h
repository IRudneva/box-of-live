#pragma once
#include <memory>
#include <TGUI/TGUI.hpp>
#include "cell.h"

constexpr static int ENERGY_ACTION_COST = 1;
constexpr static int ENERGY_BASE = 5;
constexpr static int ENERGY_TO_CLONE = 8;
constexpr static time_t UPDATE_TIME = 2;
constexpr static int ENERGY_GRASS = 3;


class Bacterium: public Cell {
public:
	explicit Bacterium(unsigned int id_type);
	
	void update(FieldState& cells) override;
	
	void setIdType(unsigned int id) { id_type_ = id; }

	unsigned int getIdType() const { return id_type_; }

	void setEnergy(unsigned int energy) { energy_base_ = energy; }

	unsigned int getEnergy() const { return energy_base_; }

	void setSpeed(unsigned int speed) { speed_ = speed; }

	unsigned int getSpeed() const { return speed_; }

private:
	unsigned int id_type_ = 0;
	int energy_base_ = ENERGY_BASE;
	int speed_ = UPDATE_TIME;
	time_t last_action_time_;

	bool isReadyUpdate() const;

	bool canClone() const { return energy_base_ >= ENERGY_TO_CLONE; }
	
	bool tryEatAnotherBacterium(std::shared_ptr<Cell> another_bacterium);

	bool checkAllBacteriaAroundSameType(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& adj_cells) const;
	
	void eatGrass(std::shared_ptr<Cell> grass);

	void spendEnergy(unsigned int count);

	void increaseEnergy(unsigned int count) { energy_base_ += count; }

	void changeDirection(FieldState& field_state);

	int findPriorytyCell(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& adj_cells, TypeCell type) const;

	std::shared_ptr<Bacterium> clone(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& adj_cells);

	Position findEmptyCell(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& adj_cells) const;
};




