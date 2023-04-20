#include "pch_server.h"
#include "field_state.h"
#include "bacterium.h"
#include "grass.h"
#include "log_duration.h"

void FieldState::addColonyBacterium(int max_count)
{
	srand(static_cast<unsigned int>(time(NULL)));
	for (unsigned int id_bacterium = 0; id_bacterium < NUMBER_BACTERIAL_COLONIES; id_bacterium++)
	{
		auto base_bacterium = std::make_shared<Bacterium>(id_bacterium, config_); // ������� ������� ��������

		Position base_bac_position = getRandomEmptyPosition(); // ������� ��� ��� ������ ������

		base_bac_position.delta_ = delta_field_size_;

		base_bacterium->setPosition(base_bac_position); // ����������� �������� ��� �������

		addBacterium(base_bacterium);

		int count_bacterium = 1; // ������� ���-�� �������� � �������

		const int max_adjacent = 3; // ������������ ���-�� ������� ��� ����� ������
		int curr_adjacent = 0; // ������� ���-�� �������

		const auto colony_size = getRandomInt(10, max_count);

		while (count_bacterium < colony_size) {

			auto adjacent_position = getRandomEmptyAdjacent(base_bac_position); // �������� ������� ������ ������
			if (adjacent_position == base_bac_position) // ��� ��������� �������� �������
			{
				base_bac_position = base_bac_position.getRandomDirection(); // ������� ���������� ����� �����
				continue; // ���� �� ����� �������� ������
			}

			auto adjacent = std::make_shared<Bacterium>(id_bacterium, config_); // ������ �������� ������

			adjacent->setPosition(adjacent_position); // ����������� �������� ��������� �������

			addBacterium(adjacent);

			count_bacterium++; //����������� ������� ��������
			curr_adjacent++; //����������� ������� �������

			if (curr_adjacent == max_adjacent) // ����� ������� ������ max_adjacent, ������� �������� ������ ��������� ������� �����������
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
	if (cells_.find(bacterium->getIdCell()) == cells_.end())
	{
		cells_.insert({ bacterium->getIdCell(), bacterium });

		delta_state_.update_cells.push_back(bacterium->getPosition());
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


		delta_state_.update_cells.push_back(new_grass->getPosition());
		++count;
	}
}

void FieldState::addGrass(int x, int y)
{
	Position pos = {x, y};
	auto new_grass = std::make_shared<Grass>();
	new_grass->setPosition(pos);
	cells_.insert({ new_grass->getIdCell(), new_grass });


	delta_state_.update_cells.push_back(new_grass->getPosition());

}

const DeltaGameState FieldState::getDeltaGameState()
{
	const auto buff_delta = delta_state_;
	delta_state_.clear();
	return buff_delta;
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
	{
		//LOG_DURATION("FIELD_STATE::UPDATE CELLS");
		for (const auto&[id, cell] : cells_)
		{
			if (cell != nullptr) {
				cell->update(*this);
				if (cell->isReadyToReset())
					resetCell(id);
			}
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
	addColonyBacterium(20);
	addGrass(config_->count_grass * 2);

}

Position FieldState::getRandomEmptyPosition() const
{
	Position position = getRandomPosition(delta_field_size_); // �������� ��������� ������� �� ����
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