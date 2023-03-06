﻿#include "field_state.h"
#include "bacterium.h"
#include "grass.h"
#include "thread"

void FieldState::addColonyBacterium(int max_count)
{
	srand(time(NULL));
	for (unsigned int id_bacterium = 0; id_bacterium < NUMBER_BACTERIAL_COLONIES; id_bacterium++)
	{
		auto base_bacterium = std::make_shared<Bacterium>(id_bacterium); // создаем базовую бактерию

		Position base_bac_position = getRandomEmptyPosition(); // находим для нее пустую клетку

		base_bacterium->setPosition(base_bac_position); // присваиваем бактерии эту позицию

		addBacterium(base_bacterium);

		int count_bacterium = 1; // текущее кол-во бактерий в колонии

		const int max_adjacent = 3; // максимальное кол-во соседей для одной клетки
		int curr_adjacent = 0; // текущее кол-во соседей

		const auto colony_size = getRandomInt(10, max_count);

		while (count_bacterium < colony_size) {

			auto adjacent_position = getRandomEmptyAdjacent(base_bac_position); // получаем позицию нового соседа
			if(adjacent_position == base_bac_position) // нет свободных соседний позиций
			{
				base_bac_position = base_bac_position.getRandomDirection(); // базовой становится любой сосед
				continue; // пока не найдём свобоную клетку
			}

			auto adjacent = std::make_shared<Bacterium>(id_bacterium); // создаём бактерию соседа

			adjacent->setPosition(adjacent_position); // присваиваем бактерии найденную позицию

			addBacterium(adjacent);
			
			count_bacterium++; //увеличиваем счётчик бактерий
			curr_adjacent++; //увеличиваем счётчик соседей

			if (curr_adjacent == max_adjacent) // когда соседей станет max_adjacent, базовой позицией станет последняя успешно добавленная
			{
				auto last_pos = adjacent->getPosition();
				base_bac_position = last_pos;
				curr_adjacent = 0;
			}
		}
	}
}

void FieldState::addBacterium(std::shared_ptr<Cell> bacterium)
{
	if(cells_.find(bacterium->getIdCell()) == cells_.end())
	{
		cells_.insert({ bacterium->getIdCell(), bacterium });
	}
}

void FieldState::resetCell(int id_cell)
{
	if (cells_.find(id_cell) != cells_.end())
	{
		cells_.at(id_cell) = nullptr;
	}
}

void FieldState::addGrass(int amount_grass)
{
	int count = 0;
	srand(time(NULL));
	while (count < amount_grass)
	{
		Position new_position = getRandomEmptyPosition();
		auto new_grass = std::make_shared<Grass>();
		new_grass->setPosition(new_position);
		cells_.insert({ new_grass->getIdCell(), new_grass });
		++count;
	}
}

void FieldState::update()
{
	if (cells_.empty())
		return;

	if (timer_grass_.timedOut())
		addGrass(50);
	
	for (const auto&[id, cell] : cells_)
	{
		if(cell != nullptr)
			cell->update(*this);
	}

	for (auto it = cells_.begin(); it != cells_.end(); )
	{
		it->second == nullptr ? it = cells_.erase(it) : ++it;
	}
}

void FieldState::restart()
{
	cells_.clear();
	IdCell::reset();
	addColonyBacterium(20);
	addGrass(300);
}

Position FieldState::getRandomEmptyPosition() const
{
	Position position = getRandomPosition(); // получаем рандомную позицию на поле
	for(const auto& [id, cell] : cells_)
	{
		if(cell->getPosition()==position)
			position = getRandomPosition();
	}
	
	return position;
}

Position FieldState::getRandomEmptyAdjacent(Position position) const
{
	auto adj_cells = getPositionsAround(position);
	
	std::vector<Position> empty_cells;
	for (const auto&[pos, cell] : adj_cells)
	{
		if (cell == nullptr) {
			empty_cells.push_back(pos);
		}
	}
	if (!empty_cells.empty())
	{
		auto rand_pos = *(empty_cells.begin() + getRandomInt(0, empty_cells.size() - 1));
		return rand_pos;
	}
	return position;
}

std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> FieldState::getPositionsAround(Position pos) const
{
	std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> adjacent_cells;
	auto all_adj_pos = pos.getAllAdjacentPosition();
	for(const auto& adj:all_adj_pos)
	{
		adjacent_cells.insert({ adj, nullptr });
	}

	for (const auto&[id, cell] : cells_)
	{
		if(cell == nullptr)
			continue;
		
		auto is_found = std::find(all_adj_pos.begin(), all_adj_pos.end(), cell->getPosition());
		
		if (is_found != all_adj_pos.end())
		{
			if (*is_found == pos)
				continue;
			
			adjacent_cells[cell->getPosition()] =  cell;
		}
	}
	return adjacent_cells;
}