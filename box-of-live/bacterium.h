#pragma once
#include <unordered_map>
#include "cell.h"
#include "config_helper.h"

using Sec = std::chrono::seconds;
using Millisec = std::chrono::milliseconds;
using TimePoint = std::chrono::steady_clock::time_point;
using AdjacentCellsUMap = std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>;

class Bacterium: public Cell {
public:
	explicit Bacterium(int id_type, std::shared_ptr<GameConfig> config);
	
	void update(FieldState& cells) override;

public:
	void setIdType(int id) { id_type_ = id; }

	void setEnergy(int energy) { energy_base_ = energy; }

	int getIdType() const { return id_type_; }

	int getEnergy() const { return energy_base_; }
	
private:
	int id_type_ = 0;
	int energy_base_ = 0;
	TimePoint last_action_time_;
	std::shared_ptr<GameConfig> config_;

private:
	bool isReadyUpdate();

	bool canClone() const { return energy_base_ >= config_->energy_to_clone; }
	
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




