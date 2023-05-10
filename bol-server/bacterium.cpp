#include "pch_server.h"
#include "bacterium.h"

#include "bol_timer.h"
#include "effect.h"
#include "field_state.h"
#include "grass.h"

Bacterium::Bacterium(int id_type, std::shared_ptr<GameConfig> config) :id_type_(id_type), config_(config)
{
	setCellType(TypeCell::BACTERIUM);
	energy_base_ = config_->energy_base;
	update_time_ = getRandomInt(config_->min_update_time, config_->max_update_time);
}

bool Bacterium::isReadyUpdate()
{
	if (auto time = std::chrono::duration_cast<Millisec>(getCurrentTime() - last_action_time_); time >= Millisec(Sec(update_time_)))
	{
		last_action_time_ = std::chrono::steady_clock::now();
		return true;
	}
	return false;
}

void Bacterium::spendEnergy(int count)
{
	if (energy_base_ >= count && can_spend_energy_)
		energy_base_ -= count;
}

void Bacterium::update(FieldState& field_state)
{
	if (is_created_now_)
	{
		setUpdateTime(update_time_);
		is_created_now_ = false;
		return;
	}

	onUpdate(*this, field_state);

	updateEffects();

	if (!isReadyUpdate())
		return;

	spendEnergy(config_->energy_action_cost);

	if (energy_base_ <= 0) {
		field_state.resetTypeCell(getIdCell());
		// удаляем, если закончилась энергия
		field_state.delta_state_.addDeletedPosition(position_);
	}

	auto old_pos = position_;
	if (auto new_pos = changeDirection(field_state); old_pos != new_pos)
	{
		field_state.delta_state_.addDeletedPosition(old_pos);
		field_state.delta_state_.addUpdatePosition(new_pos);
	}
}

void Bacterium::updateEffects()
{
	effects_.erase(std::remove_if(effects_.begin(), effects_.end(), [this](auto effect)
	{
		if (effect->isOverAction())
		{
			effect->detachEffect(*this);
			return true;
		}
		return false;
	}), effects_.end());
}

const Position& Bacterium::changeDirection(FieldState& field_state)
{
	const auto all_adjacent = field_state.getPositionsAround(position_);

	if (checkAllBacteriumAroundSameType(all_adjacent)) // если все соседние бактерии такого же типа
		return position_;

	const auto id_bacterium = findPriorytyCell(all_adjacent, TypeCell::BACTERIUM);
	if (id_bacterium != NO_RESULT)
	{
		if (tryEatAnotherBacterium(field_state.getData().at(id_bacterium))) // если получилось съесть бауктерию другого типа
		{
			onEatAnotherBacterium(*this, config_->energy_from_grass);
			field_state.resetTypeCell(id_bacterium);
			return position_;
		}
	}

	const auto id_grass = findPriorytyCell(all_adjacent, TypeCell::GRASS);
	if (id_grass != NO_RESULT)
	{
		eatGrass(field_state, id_grass);
	
		field_state.resetTypeCell(id_grass);
		if (canClone())
		{
			const auto new_all_adjacent = field_state.getPositionsAround(position_);
			auto new_bac = clone(new_all_adjacent);
			if (new_bac != nullptr) {
				new_bac->setEnergy(static_cast<int>(energy_base_ * 0.5));
				spendEnergy(static_cast<int>(energy_base_ * 0.5));
				field_state.addBacterium(new_bac);
			}
		}
		return position_;
	}

	auto pos_empty_cell = findEmptyCell(all_adjacent);

	position_ = pos_empty_cell;
	return position_;

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
				const auto bacterium = std::dynamic_pointer_cast<Bacterium>(cell);
				if (bacterium->getIdType() == id_type_)
					continue;
			}
			return cell->getIdCell();
		}
	}
	return NO_RESULT;
}

bool Bacterium::tryEatAnotherBacterium(std::shared_ptr<Cell> another_bacterium)
{
	const auto bacterium = std::dynamic_pointer_cast<Bacterium>(another_bacterium);
	if (energy_base_ > bacterium->getEnergy())
	{
		position_ = bacterium->getPosition();
		spendEnergy(static_cast<int>(bacterium->getEnergy() * 0.5));
		return true;
	}
	return false;
}

void Bacterium::eatGrass(FieldState& field_state, int id_grass)
{
	auto cell = field_state.getData().at(id_grass);
	const auto grass = std::dynamic_pointer_cast<Grass>(cell);
	if (grass->isSuperGrass())
	{
		addRandomEffect();
	}

	position_ = grass->getPosition();
	increaseEnergy(config_->energy_from_grass);
}

std::shared_ptr<Bacterium> Bacterium::clone(const AdjacentCellsUMap& adj_cells)
{
	auto child_pos = findEmptyCell(adj_cells);

	if (child_pos != position_)
	{
		auto child_bacterium = std::make_shared<Bacterium>(id_type_, config_);
		child_bacterium->setPosition(child_pos);
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
	for (const auto&[pos, cell] : adj_cells)
	{
		if (cell == nullptr)
			break;
		if (cell->getCellType() == TypeCell::GRASS)
			break;
		if (cell->getCellType() == TypeCell::BACTERIUM)
		{
			const auto bacterium = std::dynamic_pointer_cast<Bacterium>(cell);
			if (bacterium->getIdType() == id_type_)
				++count;
		}
	}
	if (count == adj_cells.size())
		return true;
	return false;
}