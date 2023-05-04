#include "pch_server.h"
#include "field_state.h"
#include "bacterium.h"
#include "grass.h"
#include "log_duration.h"

void DeltaGameState::addDeletedPosition(const Position& pos)
{
	if (auto cell = std::find(deleted_cells_.begin(), deleted_cells_.end(), pos); cell == deleted_cells_.end())
		deleted_cells_.push_back(pos);
}

void DeltaGameState::addUpdatePosition(const Position& pos)
{
	if (auto cell = std::find(update_cells_.begin(), update_cells_.end(), pos); cell == update_cells_.end())
		update_cells_.push_back(pos);
}

void DeltaGameState::clear()
{
	deleted_cells_.clear();
	update_cells_.clear();
}


void FieldState::addColonyBacterium(int max_count)
{
	srand(static_cast<unsigned int>(time(NULL)));
	for (unsigned int id_bacterium = 1; id_bacterium < NUMBER_BACTERIAL_COLONIES; id_bacterium++)
	{
		auto base_bacterium = std::make_shared<Bacterium>(id_bacterium, config_); // создаем базовую бактерию

		Position base_bac_position = getRandomEmptyPosition(); // находим для нее пустую клетку

		base_bac_position.delta_ = delta_field_size_;

		base_bacterium->setPosition(base_bac_position); // присваиваем бактерии эту позицию

		addBacterium(base_bacterium);

		int count_bacterium = 1; // текущее кол-во бактерий в колонии

		const int max_adjacent = 3; // максимальное кол-во соседей для одной клетки
		int curr_adjacent = 0; // текущее кол-во соседей

		const auto colony_size = getRandomInt(6, max_count);

		while (count_bacterium < colony_size) {

			auto adjacent_position = getRandomEmptyAdjacent(base_bac_position); // получаем позицию нового соседа
			if (adjacent_position == base_bac_position) // нет свободных соседний позиций
			{
				base_bac_position = base_bac_position.getRandomDirection(); // базовой становится любой сосед
				continue; // пока не найдём свобоную клетку
			}

			auto adjacent = std::make_shared<Bacterium>(id_bacterium, config_); // создаём бактерию соседа

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

void FieldState::addBacterium(int x, int y, int id_type, int energy)
{
	auto bacterium = std::make_shared<Bacterium>(id_type, config_); // создаем базовую бактерию

	bacterium->setPosition(Position{ x, y, delta_field_size_ }); // присваиваем бактерии эту позицию

	bacterium->setEnergy(energy);

	addBacterium(bacterium);
}

void FieldState::addBacterium(std::shared_ptr<Cell> bacterium)
{
	if (cells_.find(bacterium->getIdCell()) == cells_.end())
	{
		cells_.insert({ bacterium->getIdCell(), bacterium });

		delta_state_.addUpdatePosition(bacterium->getPosition());
	}
}

void FieldState::resetTypeCell(int id_cell) const
{
	if (cells_.find(id_cell) != cells_.end())
		cells_.at(id_cell)->setCellType(TypeCell::EMPTY);
}

void FieldState::resetCell(int id_cell)
{
	if (const auto cell = cells_.find(id_cell); cell != cells_.end())
		cells_.at(id_cell) = nullptr;
}

void FieldState::addGrass(int amount_grass)
{
	int count = 0;
	srand(static_cast<unsigned int>(time(NULL)));
	while (count < amount_grass)
	{
		Position new_position = getRandomEmptyPosition();
		auto new_grass = std::make_shared<Grass>();
		new_grass->setPosition(new_position);
		cells_.insert({ new_grass->getIdCell(), new_grass });


		delta_state_.addUpdatePosition(new_grass->getPosition());
		++count;
	}
}

void FieldState::addGrass(int x, int y)
{
	Position pos = {x, y};
	auto new_grass = std::make_shared<Grass>();
	new_grass->setPosition(pos);
	cells_.insert({ new_grass->getIdCell(), new_grass });


	delta_state_.addUpdatePosition(new_grass->getPosition());

}

std::shared_ptr<Cell> FieldState::getCellInPosition(const Position& pos) const
{
	for (const auto&[id, cell] : cells_)
	{
		if (cell->getPosition() == pos)
			return cell;
	}
	return nullptr;
}

void FieldState::initConfig(std::shared_ptr<GameConfig> config)
{
	config_ = std::move(config);
	delta_field_size_ = static_cast<double>(config_->delta_game_field_size) / 10;
	
}

void FieldState::update()
{
	if (cells_.empty())
		return;

	if (timer_grass_.timedOut())
		addGrass(config_->count_grass);

	for (const auto&[id, cell] : cells_)
	{
		if (cell != nullptr) {
			cell->update(*this);
			if (cell->isReadyToReset())
				resetCell(id);
		}
	}

	for (auto it = cells_.begin(); it != cells_.end(); )
	{
		it->second == nullptr ? it = cells_.erase(it) : ++it;
	}
}

void FieldState::restart()
{
	cells_.clear();
	delta_state_.clear();
	IdCell::reset();
	timer_grass_.initInt(config_->grass_update_time);
	addColonyBacterium(15);
	addGrass(config_->count_grass * 2);
}

Position FieldState::getRandomEmptyPosition() const
{
	Position position = getRandomPosition(delta_field_size_); // получаем рандомную позицию на поле
	for (const auto&[id, cell] : cells_)
	{
		if (cell->getPosition() == position)
			position = getRandomPosition(delta_field_size_);
	}

	return position;
}

Position FieldState::getRandomEmptyAdjacent(const Position& position) const
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
		auto rand_pos = *(empty_cells.begin() + getRandomInt(0, static_cast<int>(empty_cells.size() - 1)));
		return rand_pos;
	}
	return position;
}

AdjacentCellsUMap FieldState::getPositionsAround(const Position& pos) const
{
	std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> adjacent_cells;
	auto all_adj_pos = pos.getAllAdjacentPosition();
	for (const auto& adj : all_adj_pos)
	{
		adjacent_cells.insert({ adj, nullptr });
	}
	for (const auto&[id, cell] : cells_)
	{
		if (cell == nullptr)
			continue;

		auto is_found = std::find(all_adj_pos.begin(), all_adj_pos.end(), cell->getPosition());

		if (is_found != all_adj_pos.end())
		{
			if (*is_found == pos)
				continue;
			adjacent_cells[cell->getPosition()] = cell;
		}
	}
	return adjacent_cells;
}