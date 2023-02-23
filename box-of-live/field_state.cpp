#include "field_state.h"
#include "bacterium.h"
#include "grass.h"

void FieldState::addBacterium(unsigned int max_count)
{
	srand(time(NULL));
	for (unsigned int id_bac = 0; id_bac < NUMBER_BACTERIAL_COLONIES; id_bac++) {
		Position pos_base_bac;
		while (true)
		{//позиция первой бактерии для колонии
			auto rand_pos(getRandomPosition());
			pos_base_bac = rand_pos;
			if (data_cell_.find(pos_base_bac) != data_cell_.end())
				continue;
			data_cell_[pos_base_bac] = std::make_shared<Bacterium>(id_bac);
			break;
		}
		//строим колонию
		unsigned int rcount = getRandomUInt(15, max_count); //кол-во бактерий(клеток, которые нужно заполнить) (рандом от 15 до 30)

		Position base = pos_base_bac; // Позиция начала колонии 
		unsigned int min_neigh = 4; //  Минимальное кол-во соседей у одной бактерии, кол-во клеток, которые нужно заполнить

		for (unsigned int i = 0; i <= rcount;) {
			auto neigh_pos = base.getRandomDirection();

			auto new_bac = std::make_shared<Bacterium>(id_bac);
			data_cell_.insert({ neigh_pos,std::move(new_bac) });// Пока у базовой клетки не будет 4х соседнтх бактерий, не меняем базовую клетку

			if (i == min_neigh) {
				base = neigh_pos; // теперь базовой будет последняя рандомная соседняя клетка
			}

			i++;
		}
	}
}

void FieldState::addGrass(unsigned int amount_grass)
{
	srand(time(NULL));
	while (data_cell_.size() <= amount_grass) {
		Position new_position(getRandomPosition());
		auto new_grass = std::make_shared<Grass>();
		data_cell_.insert({ new_position, std::move(new_grass) });
	}

}

void FieldState::update()
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
