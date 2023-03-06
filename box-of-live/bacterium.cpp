#include "bacterium.h"

Bacterium::Bacterium(unsigned int id_type)
	:id_type_(id_type)
{
	setCellType(TypeCell::BACTERIUM);
}

bool Bacterium::isReadyUpdate() const
{
	if (getCurrentTime() - last_action_time_ >= UPDATE_TIME)
	{
		return true;
	}
	return false;
}


void Bacterium::spendEnergy(unsigned int count)
{
	if (energy_base_ >= count)
		energy_base_ -= count;
}
void Bacterium::update(FieldState& field_state)
{
	/*алгоритм бактерии :
	1. проверить что время делать действие
		2. уменьшить энергию
		3. выбрать клетку куда идти
		4. в зависимости от результата сделать действие :
	-съесть траву - возможно создать новую бактерию
		- съесть другую бактерию
		- переместиться
		- ничего не делать
		5. проверить что количество энергии больше 0
		- если нет - удалить бактерию*/
	
	if (isReadyUpdate())
	{
		spendEnergy(ENERGY_ACTION_COST); //тратит энергию 
		changeDirection(field_state);

		if(energy_base_ <= 0)
		{
			field_state.resetCell(getIdCell());
			return;
		}
		last_action_time_ = getCurrentTime();
	}

}

void Bacterium::changeDirection(FieldState& field_state)
{
	const auto all_adjacent = field_state.getPositionsAround(position_); // смотрим соседей

	if (checkAllBacteriaAroundSameType(all_adjacent))
		return;
	
	auto id_bacterium = findPriorytyCell(all_adjacent, TypeCell::BACTERIUM);
	if (id_bacterium != NO_RESULT)
	{
		if (tryEatAnotherBacterium(field_state.getData().at(id_bacterium)))
		{
			field_state.resetCell(id_bacterium);
			return;
		}
	}
	
	auto id_grass = findPriorytyCell(all_adjacent, TypeCell::GRASS);
	if(id_grass!= NO_RESULT)
	{
		eatGrass(field_state.getData().at(id_grass));
		field_state.resetCell(id_grass);
		if (canClone())
		{
			const auto new_all_adjacent = field_state.getPositionsAround(position_);
			auto new_bac = clone(new_all_adjacent);
			if(new_bac != nullptr)
				field_state.addBacterium(new_bac);
		}
		return;
	}
	
	auto pos_empty_cell = findEmptyCell(all_adjacent);
	position_ = pos_empty_cell;
}

int Bacterium::findPriorytyCell(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& adj_cells, TypeCell type) const
{
	for (const auto&[pos, cell] : adj_cells)
	{
		if (cell == nullptr)
			continue;
		if (cell->getCellType() == type)
		{
			if (type == TypeCell::BACTERIUM) 
			{
				Cell& c = *cell;
				const auto bacterium = dynamic_cast<Bacterium&>(c);
				if (bacterium.getIdType() == id_type_)
					continue;
			}
			return cell->getIdCell();
		}
	}
	return NO_RESULT;
}

bool Bacterium::tryEatAnotherBacterium(std::shared_ptr<Cell> another_bacterium)
{
	Cell& cell = *another_bacterium;
	const auto bacterium = dynamic_cast<Bacterium&>(cell);
	if (energy_base_ > bacterium.getEnergy())
	{
		position_ = cell.getPosition();
		spendEnergy(bacterium.getEnergy() * 0.5);
		return true;
	}
	return false;
}

void Bacterium::eatGrass(std::shared_ptr<Cell> grass)
{
	position_ = grass->getPosition();
	increaseEnergy(ENERGY_GRASS);
}

std::shared_ptr<Bacterium> Bacterium::clone(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& adj_cells)
{
	auto child_pos = findEmptyCell(adj_cells);
	
	if (child_pos != position_) 
	{
		auto child_bacterium = std::make_shared<Bacterium>(id_type_);
		child_bacterium->setPosition(child_pos);
		child_bacterium->setEnergy(energy_base_ * 0.5);
		spendEnergy(energy_base_*0.5);
		return child_bacterium;
	}
	return nullptr;
}

Position Bacterium::findEmptyCell(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& adj_cells) const
{
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
	return position_;
}

bool Bacterium::checkAllBacteriaAroundSameType(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& adj_cells) const
{
	int count = 0;
	for (const auto& [pos, cell]: adj_cells)
	{
		if (cell== nullptr)
			break;
		if (cell->getCellType() == TypeCell::GRASS)
			break;
		if (cell->getCellType() == TypeCell::BACTERIUM)
		{
			Cell& c = *cell;
			const auto bacterium = dynamic_cast<Bacterium&>(c);
			if(bacterium.getIdType() == id_type_)
			{
				++count;
			}
		}
	}
	if(count == adj_cells.size())
		return true;
	return false;
}