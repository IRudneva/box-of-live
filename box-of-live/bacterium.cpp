#include "bacterium.h"

bool Bacterium::isReadyUpdate()
{
	if (auto time = std::chrono::duration_cast<Millisec>(getCurrentTime() - last_action_time_); time >= Millisec(Sec(config_->update_time)))
	{
		last_action_time_ = getCurrentTime();
		return true;
	}
	return false;
}

void Bacterium::spendEnergy(int count)
{
	if (energy_base_ >= count)
		energy_base_ -= count;
}

void Bacterium::update(FieldState& field_state)
{
	if (isReadyUpdate())
	{
		spendEnergy(config_->energy_action_cost);
		changeDirection(field_state);

		if(energy_base_ <= 0)
		{
			field_state.resetCell(getIdCell());
			return;
		}
	}
}

void Bacterium::changeDirection(FieldState& field_state)
{
	const auto all_adjacent = field_state.getPositionsAround(position_);

	if (checkAllBacteriumAroundSameType(all_adjacent))
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

int Bacterium::findPriorytyCell(const AdjacentCellsUMap& adj_cells, TypeCell type) const
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
		spendEnergy(static_cast<int>(bacterium.getEnergy() * 0.5));
		return true;
	}
	return false;
}

void Bacterium::eatGrass(std::shared_ptr<Cell> grass)
{
	position_ = grass->getPosition();
	increaseEnergy(config_->energy_from_grass);
}

std::shared_ptr<Bacterium> Bacterium::clone(const AdjacentCellsUMap& adj_cells)
{
	auto child_pos = findEmptyCell(adj_cells);
	
	if (child_pos != position_) 
	{
		auto child_bacterium = std::make_shared<Bacterium>(id_type_ , config_);
		child_bacterium->setPosition(child_pos);
		child_bacterium->setEnergy(static_cast<int>(energy_base_ * 0.5));
		spendEnergy(static_cast<int>(energy_base_ * 0.5));
		return child_bacterium;
	}
	return nullptr;
}

Position Bacterium::findEmptyCell(const AdjacentCellsUMap& adj_cells) const
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
		auto rand_pos = *(empty_cells.begin() + getRandomInt(0, static_cast<int>(empty_cells.size() - 1)));
		return rand_pos;
	}
	return position_;
}

bool Bacterium::checkAllBacteriumAroundSameType(const AdjacentCellsUMap& adj_cells) const
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
				++count;
		}
	}
	if(count == adj_cells.size())
		return true;
	return false;
}