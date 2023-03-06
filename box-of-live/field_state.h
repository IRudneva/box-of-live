#pragma once
#include "cell.h"
class Cell;


class FieldState
{
public:
	void init(GameConfig& config) { config_ = config;  }

	void update();

	const std::map<int, std::shared_ptr<Cell>>& getData() const { return cells_; }

	void restart();

	void addBacterium(std::shared_ptr<Cell> bacterium);

	void resetCell(int id_cell);
	
	std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> getPositionsAround(Position pos) const;

private:
	GameConfig config_;
	std::map<int, std::shared_ptr<Cell>> cells_;
	Timer timer_grass_ = {5};

	Position getRandomEmptyPosition() const;

	Position getRandomEmptyAdjacent(Position position) const;

	void addColonyBacterium(int max_count);

	void addGrass(int amount_grass);
};

