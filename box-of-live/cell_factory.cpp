#include "cell_factory.h"
#include "bacterium.h"
#include "grass.h"

CellFactory::CellFactory() {
	for (unsigned int x = 0; x <= WIDTH_PLAYING_FIELD;)
	{
		data_positions_.coord_x.push_back(x);
		x += CELL_SIZE;
	}
	for (unsigned int y = 0; y <= HEIGHT_PLAYING_FIELD;)
	{
		data_positions_.coord_y.push_back(y);
		y += CELL_SIZE;
	}
}

void CellFactory::addBacterium(unsigned int amount_species, unsigned int max_count)
{
	srand(time(NULL));
	Position pos_base_bac;
	while (true)
	{//позиция первой бактерии для колонии
		unsigned int rnum_x = getRandInt(0, data_positions_.coord_x.size() - 1);
		unsigned int rnum_y = getRandInt(0, data_positions_.coord_y.size() - 1);
		pos_base_bac = { data_positions_.coord_x[rnum_x] , data_positions_.coord_y[rnum_y] };
		if (data_cell_.find(pos_base_bac) != data_cell_.end())
			continue;
		data_cell_[pos_base_bac] = std::make_shared<Bacterium>();
		data_cell_[pos_base_bac]->setPosition(pos_base_bac);
		break;
	}
	//строим колонию
	unsigned int rcount = getRandInt(15, max_count); //кол-во бактерий(клеток, которые нужно заполнить) (рандом от 15 до 30)

	Position base = pos_base_bac; // Позиция начала колонии 
	unsigned int min_neigh = 4; //  Минимальное кол-во соседей у одной бактерии, кол-во клеток, которые нужно заполнить

	for (unsigned int i = 0; i <= rcount;) {
		std::vector<Position> neighbour_pos = { // 8 соседних позиций у базовой клетки
		{base.x - CELL_SIZE, base.y + CELL_SIZE},
		{base.x, pos_base_bac.y + CELL_SIZE},
		{base.x + CELL_SIZE, base.y + CELL_SIZE},
		{ base.x + CELL_SIZE, base.y},
		{ base.x + CELL_SIZE, base.y - CELL_SIZE},
		{ base.x, base.y - CELL_SIZE},
		{ base.x - CELL_SIZE, base.y - CELL_SIZE} ,
		{ base.x - CELL_SIZE, base.y}
		};
		//рандомный сосед
		unsigned int rnum_neig = getRandInt(0, neighbour_pos.size() - 1);

		auto neigh_pos = neighbour_pos[rnum_neig]; // позиция рандомного соседа

		auto new_bac = std::make_shared<Bacterium>();
		new_bac->setPosition(neigh_pos);
		data_cell_.insert({ neigh_pos,std::move(new_bac) });// Пока у базовой клетки не будет 4х соседнтх бактерий, не меняем базовую клетку

		if (i == min_neigh) { 
			base = neigh_pos; // теперь базовой будет последняя рандомная соседняя клетка
		}

		i++;
	}
}

void CellFactory::addGrass(unsigned int amount_grass)
{
	srand(time(NULL));
	while (data_cell_.size() <= amount_grass) {
		unsigned int rnum_x = getRandInt(0, data_positions_.coord_x.size() - 1);
		unsigned int rnum_y = getRandInt(0, data_positions_.coord_y.size() - 1);

		Position new_position{ data_positions_.coord_x[rnum_x] , data_positions_.coord_y[rnum_y] };
		auto new_grass = std::make_shared<Grass>();
		new_grass->setPosition(new_position);
		data_cell_.insert({ new_position, std::move(new_grass) });
	}

}

void CellFactory::update()
{
	/*if(data_cell_.empty())
		return;

	unsigned int rand_cell = rand() % data_cell_.size();
	auto it_data_cell = data_cell_.begin();
	std::advance(it_data_cell, rand_cell);

	auto pos_cell = (*it_data_cell).first;

	std::vector<Position> neighbour_pos = {
		{pos_cell.x - CELL_SIZE, pos_cell.y + CELL_SIZE},
		{pos_cell.x, pos_cell.y + CELL_SIZE},
		{pos_cell.x + CELL_SIZE, pos_cell.y + CELL_SIZE},
		{pos_cell.x + CELL_SIZE, pos_cell.y},
		{pos_cell.x + CELL_SIZE, pos_cell.y - CELL_SIZE},
		{pos_cell.x, pos_cell.y - CELL_SIZE},
		{pos_cell.x - CELL_SIZE, pos_cell.y - CELL_SIZE} ,
		{pos_cell.x - CELL_SIZE, pos_cell.y}
	};

	unsigned int rnum_neig = rand() % neighbour_pos.size();
	auto neigh_pos = neighbour_pos[rnum_neig];
	data_cell_[neigh_pos] = std::make_shared<Bacterium>();
	data_cell_[neigh_pos]->setPosition(neigh_pos);*/

	/*	for (auto i = 0; i < neibor_cells.size(); i++)
		{
			unsigned int rand_cell = rand() % neibor_cells.size();
			auto p = neibor_cells[rand_cell];
			if (map_.find(p) != map_.end())
			{
				continue;
			}*/
			/*map_[p] = std::make_shared<Grass>();
			map_[p]->setPosition(p);
			map_[p]->setColor(tgui::Color::Green);*/

			/*	break;
			}*/
}
