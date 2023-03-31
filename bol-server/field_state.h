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

class FieldState
{
public:
	void initConfig(std::shared_ptr<GameConfig> config) { config_ = std::move(config); }

	void update();

	void restart();

	void addBacterium(std::shared_ptr<Cell> bacterium);

	void resetTypeCell(int id_cell) const;

	void resetCell(int id_cell);

	const std::map<int, std::shared_ptr<Cell>>& getData() const { return cells_; }

	AdjacentCellsUMap getPositionsAround(const Position& pos) const;

	std::shared_ptr<GameConfig> getGameConfig() const { return config_; }

private:
	std::shared_ptr<GameConfig> config_ = std::make_shared<GameConfig>();
	std::map<int, std::shared_ptr<Cell>> cells_;
	Timer timer_grass_;


private:
	Position getRandomEmptyPosition() const;

	Position getRandomEmptyAdjacent(const Position& position) const;

	void addColonyBacterium(int max_count);

	void addGrass(int amount_grass);
};

