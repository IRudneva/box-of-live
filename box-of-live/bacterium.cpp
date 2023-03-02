#include "bacterium.h"

Bacterium::Bacterium(unsigned int id_type)
	:id_type_(id_type)
{
	setCellType(TypeCell::BACTERIUM);
}

bool Bacterium::isReadyUpdate()
{
	if (getCurrentTime() - last_action_time_ >= UPDATE_TIME)
	{
		return true;
	}
	return false;
}

void Bacterium::update(const std::map<int, std::shared_ptr<Cell>>& cells)
{
	spendEnergy(ENERGY_ACTION_COST); //тратит энергию 
	if (!canMove()) // если энергии недостаточно для перемещения - стоит на месте
		return;
	//// если енергии хватает для перемещения
	tryMovePriorityCell(cells);
	last_action_time_ = getCurrentTime();

}

bool Bacterium::canMove() const
{
	if (energy_base_ > ENERGY_ACTION_COST)
		return true;

	return false;
}

void Bacterium::spendEnergy(unsigned int count)
{
	if (energy_base_ >= count)
		energy_base_ -= count;
}

std::shared_ptr<Cell> Bacterium::clone(const std::map<int, std::shared_ptr<Cell>>& cells)
{
	auto pos_clone = tryMoveEmptyCell(cells);
	if (pos_clone != position_) {
		auto child_bacterium = std::make_shared<Bacterium>(id_type_);
		child_bacterium->setPosition(pos_clone);
		return std::move(child_bacterium);
	}
	return nullptr;
}

bool Bacterium::tryMovePriorityCell(const std::map<int, std::shared_ptr<Cell>>& cells)
{
	bool successful_move = false;

	const auto all_adjacent = position_.getAllAdjacentPosition(); // смотрим соседей
	//
	auto id_another_bacterium = tryFindBacteriumAnotherType(cells);

	if (id_another_bacterium != NO_RESULT)
	{
		Cell& another_cell = *cells.at(id_another_bacterium);
		const auto another_bacterium = dynamic_cast<Bacterium&>(another_cell);
		if (energy_base_ > another_bacterium.getEnergy())
		{
			erase_id_ = id_another_bacterium;
			position_ = another_cell.getPosition();
			spendEnergy(another_bacterium.getEnergy() * 0.5);
			successful_move = true;
			return successful_move;
		}
		// если у другой бактерии энергии больше - ищем другую позицию - траву
	}

	auto grass_id = tryFindGrass(cells);

	if (grass_id != NO_RESULT)
	{
		erase_id_ = grass_id;
		position_ = cells.at(grass_id)->getPosition();
		increaseEnergy(ENERGY_GRASS);
		successful_move = true;
		return successful_move;
	}

	auto empty_cell_pos = tryMoveEmptyCell(cells);

	if (empty_cell_pos != position_)
	{
		position_ = empty_cell_pos;
		successful_move = true;
		return successful_move;
	}

	return  successful_move;
}

int Bacterium::tryFindBacteriumAnotherType(const std::map<int, std::shared_ptr<Cell>>& cells)
{
	int count_adj = 0;
	auto all_adjacent = position_.getAllAdjacentPosition();

	for (const auto&[id, cell] : cells)
	{
		if (count_adj == all_adjacent.size())
			return -NO_RESULT;

		if (cell->getCellType() == TypeCell::BACTERIUM) {


			auto is_found = std::find(all_adjacent.begin(), all_adjacent.end(), cell->getPosition());

			if (is_found == all_adjacent.end())
				continue;

			count_adj++;
			Cell& adj_cell = *cell;
			const auto adj_bacterium = dynamic_cast<Bacterium&>(adj_cell);
			if (adj_bacterium.getIdType() != id_type_) // если хоть одна бактерия другого типа
				return adj_bacterium.getIdCell(); // возвращаем id этой бактерии
		}
	}
	return NO_RESULT;
}

int Bacterium::tryFindGrass(const std::map<int, std::shared_ptr<Cell>>& cells)
{
	auto all_adjacent = position_.getAllAdjacentPosition();

	for (const auto&[id, cell] : cells)
	{
		if (cell->getCellType() != TypeCell::GRASS)
			continue;
		auto is_found = std::find(all_adjacent.begin(), all_adjacent.end(), cell->getPosition());

		if (is_found == all_adjacent.end())
			continue;

		return id;
	}
	return NO_RESULT;
}

Position Bacterium::tryMoveEmptyCell(const std::map<int, std::shared_ptr<Cell>>& cells)
{
	int count_adj = 0;
	auto all_adjacent = position_.getAllAdjacentPosition();

	for (const auto&[id, cell] : cells)
	{
		if (count_adj == all_adjacent.size())
			return position_;

		if (id == getIdCell())
			continue;

		auto is_found = std::find(all_adjacent.begin(), all_adjacent.end(), cell->getPosition());

		if (is_found != all_adjacent.end())
		{
			all_adjacent.erase(is_found);
			count_adj++;
		}
		
	}

	if (!all_adjacent.empty())
		return *(all_adjacent.begin() + getRandomInt(0, all_adjacent.size() - 1));
	
	return position_;
}