#pragma once
#include "cell.h"
#include "position.h"
#include "game_domain.h"
#include "bol_timer.h"

class Cell;

using Millisec = std::chrono::milliseconds;
using Sec = std::chrono::seconds;
using TimePoint = std::chrono::steady_clock::time_point;
using AdjacentCellsUMap = std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>;

struct DeltaGameState
{
	const std::vector<Position>& getDeletedPositions() const { return deleted_cells_; }

	const std::vector<Position>& getUpdatedPositions() const { return  update_cells_; }

	void addDeletedPosition(const Position& pos)
	{
		if (auto cell = std::find(deleted_cells_.begin(), deleted_cells_.end(), pos); cell == deleted_cells_.end())
			deleted_cells_.push_back(pos);
	}

	void addUpdatePosition(const Position& pos)
	{
		if (auto cell = std::find(update_cells_.begin(), update_cells_.end(), pos); cell == update_cells_.end())
			update_cells_.push_back(pos);
	}

	bool empty() const
	{
		return deleted_cells_.empty() && update_cells_.empty();
	}

	void clear()
	{
		deleted_cells_.clear();
		update_cells_.clear();
	}

private:
	std::vector<Position> deleted_cells_;
	std::vector<Position> update_cells_;
};

class FieldState final
{
public:
	DeltaGameState delta_state_;

	const DeltaGameState getDeltaGameState();

	std::shared_ptr<Cell> getCellInPosition(const Position& pos) const;

	void initConfig(std::shared_ptr<GameConfig> config);

	void update();

	void restart();

	void addBacterium(std::shared_ptr<Cell> bacterium);

	void addBacterium(int x, int y, int id_type, int energy);

	void addGrass(int amount_grass);

	void addGrass(int x, int y);

	void resetTypeCell(int id_cell) const;

	void resetCell(int id_cell);

	const std::map<int, std::shared_ptr<Cell>>& getData() const { return cells_; }

	AdjacentCellsUMap getPositionsAround(const Position& pos) const;

	std::shared_ptr<GameConfig> getGameConfig() const { return config_; }

private:
	std::shared_ptr<GameConfig> config_ = std::make_shared<GameConfig>();
	using ID = int;
	std::map<ID, std::shared_ptr<Cell>> cells_;
	Timer timer_grass_{std::chrono::milliseconds(3000)};

	double delta_field_size_ = 0.0;
private:
	Position getRandomEmptyPosition() const;

	Position getRandomEmptyAdjacent(const Position& position) const;

	void addColonyBacterium(int max_count);

};

