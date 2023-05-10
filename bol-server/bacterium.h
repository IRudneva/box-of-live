#pragma once
#include <unordered_map>
#include "cell.h"
#include "game_domain.h"
#include "effect.h"
#include <boost/signals2/signal.hpp>

#include "field_state.h"

class FieldState;

using Sec = std::chrono::seconds;
using Millisec = std::chrono::milliseconds;
using TimePoint = std::chrono::steady_clock::time_point;
using AdjacentCellsUMap = std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>;

class Bacterium : public Cell, public boost::signals2::trackable {
public:
	explicit Bacterium(int id_type, std::shared_ptr<GameConfig> config);

	void update(FieldState& cells) override;

public:
	void setEnergy(int energy) { energy_base_ = energy; }

	int getIdType() const { return id_type_; }

	GameConfig getConfig() const { return config_; }

	int getEnergy() const { return energy_base_; }

	void setUpdateTime(int new_time) { update_time_ = new_time; }

	int getUpdateTime() const { return update_time_; }

	void canSpendEnergy(bool can_spend) { can_spend_energy_ = can_spend; }

	boost::signals2::signal<void(Bacterium& bacterium, int grass_energy)> onEatAnotherBacterium;

	boost::signals2::signal<void(Bacterium& bacterium, FieldState& state)> onUpdate;


private:
	int id_type_ = 0;
	int energy_base_ = 0;
	TimePoint last_action_time_;
	int update_time_ = 0;
	/*std::shared_ptr<*/GameConfig config_;

	std::vector<std::shared_ptr<Effect>> effects_;
	bool can_spend_energy_ = true;

private:
	bool isReadyUpdate();

	bool canClone() const { return energy_base_ >= config_.energy_to_clone; }

	bool tryEatAnotherBacterium(std::shared_ptr<Cell> another_bacterium);

	bool checkAllBacteriumAroundSameType(const AdjacentCellsUMap& adj_cells) const;

	void eatGrass(FieldState& field_state, int id_grass);

	void updateEffects();

	void spendEnergy(int count);

	void increaseEnergy(int count) { energy_base_ += count; }

	std::shared_ptr<Bacterium> clone(const AdjacentCellsUMap& adj_cells);

	const Position&  changeDirection(FieldState& field_state);

	int findPriorytyCell(const AdjacentCellsUMap& adj_cells, TypeCell type) const;

	Position findEmptyCell(const AdjacentCellsUMap& adj_cells) const;
};






