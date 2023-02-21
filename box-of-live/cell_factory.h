#pragma once
#include "cell.h"

class CellFactory
{
public:
	CellFactory();

	void addBacterium(unsigned int amount_species, unsigned int max_count);

	void addGrass(unsigned int amount_grass);

	void update();

	const std::unordered_map<Position, std::shared_ptr<Cell>, PosHasher>& getData() const { return data_cell_; }

private:
	std::unordered_map<Position, std::shared_ptr<Cell>, PosHasher> data_cell_;

	struct DataPositions {
		DataPositions() {
			coord_x.reserve(1000);
			coord_y.reserve(1000);
		}
		std::vector<unsigned int> coord_x;
		std::vector<unsigned int> coord_y;
	} data_positions_;
};

