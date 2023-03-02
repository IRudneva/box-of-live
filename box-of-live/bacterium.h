#pragma once
#include <memory>
#include <optional>
#include <TGUI/TGUI.hpp>
#include "cell.h"

constexpr static int ENERGY_ACTION_COST = 1;
constexpr static int ENERGY_BASE = 5;
constexpr static int ENERGY_TO_CLONE = 4;
constexpr static time_t UPDATE_TIME = 2;
constexpr static int ENERGY_GRASS = 3;


class Bacterium: public Cell {
public:
	Bacterium(unsigned int id_type)
		:id_type_(id_type)
	{
		setCellType(TypeCell::BACTERIUM);
	}

	bool isReadyUpdate() override
	{
		if(getCurrentTime() - last_action_time_ >= UPDATE_TIME)
		{
			return true;
		}
		return false;
	}

	void setIdType(unsigned int id) { id_type_ = id; }

	unsigned int getIdType() const { return id_type_; }

	void setEnergy(unsigned int energy) { energy_base_ = energy; }

	unsigned int getEnergy() const { return energy_base_; }

	void setSpeed(unsigned int speed) { speed_ = speed; }

	unsigned int getSpeed() const { return speed_; }
	   	
	void update(std::map<int, std::shared_ptr<Cell>>& cells) override
	{
		spendEnergy(ENERGY_ACTION_COST); //тратит энергию 
		if (!canMove()) // если энергии недостаточно для перемещения - стоит на месте
			return;
		//// если енергии хватает для перемещения
		tryMovePriorityCell(cells);

		
		//if (update_position.has_value())
		//{
		//	if (canClone())
		//		clone(update_position.value(), data_cell);
		//	
		//	return update_position;
		//}
		//return std::nullopt;
	}

private:
	unsigned int id_type_ = 0;
	unsigned int energy_base_ = ENERGY_BASE;
	unsigned int speed_ = UPDATE_TIME;
	time_t last_action_time_;


	bool canMove() const
	{
		if (energy_base_ > ENERGY_ACTION_COST)
			return true;

		return false;
	}

	void spendEnergy(unsigned int count)
	{
		if (energy_base_ >= count)
			energy_base_ -= count;
	}

	void increaseEnergy(unsigned int count) { energy_base_ += count; }

	bool canClone() { return energy_base_ >= ENERGY_TO_CLONE; }

	void clone(UpdateState state, const std::map<int, std::shared_ptr<Cell>>& cells)
	{/*
		auto all_adjacent = state.new_position.getAllAdjacentPosition();
		for (auto adj : all_adjacent)
		{
			if (data_cell.find(adj) == data_cell.end())
				state.chaild_position = adj;
		}
		state.chaild_position = std::nullopt;*/
	}

	/*пытается переместиться в случайную точку рядом с собой, если не получается - стоит на месте
		не может перемещаться на точку где есть бактерия такого же типа
		не может перемещаться на точку если у другой бактерии энергии больше
		приоритет выбора точек : бактерия другого типа, трава, пустая клетка*/

	bool tryMovePriorityCell(const std::map<int, std::shared_ptr<Cell>>& cells)
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

		if(grass_id != NO_RESULT)
		{
			erase_id_ = grass_id;
			position_ = cells.at(grass_id)->getPosition();
			increaseEnergy(ENERGY_GRASS);
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

	int tryFindBacteriumAnotherType(const std::map<int, std::shared_ptr<Cell>>& cells)
	{
		int count_adj = 0;
		auto all_adjacent = position_.getAllAdjacentPosition();

		for (const auto&[id, cell] : cells)
		{
			if (count_adj == all_adjacent.size())
				return -NO_RESULT;

			if (cell->getCellType() != TypeCell::BACTERIUM)
				continue;

			auto is_found = std::find(all_adjacent.begin(), all_adjacent.end(), cell->getPosition());

			if (is_found == all_adjacent.end())
				continue;

			count_adj++;
			Cell& adj_cell = *cell;
			const auto adj_bacterium = dynamic_cast<Bacterium&>(adj_cell);
			if (adj_bacterium.getIdType() != id_type_) // если хоть одна бактерия другого типа
				return adj_bacterium.getIdCell(); // возвращаем id этой бактерии 

			return NO_RESULT;
		}
	}

	int tryFindGrass(const std::map<int, std::shared_ptr<Cell>>& cells)
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

	Position tryMoveEmptyCell(const std::map<int, std::shared_ptr<Cell>>& cells)
	{
		int count_adj = 0;
		auto all_adjacent = position_.getAllAdjacentPosition();

		for (const auto&[id, cell] : cells)
		{
			if (count_adj == all_adjacent.size())
				return position_;

			auto is_found = std::find(all_adjacent.begin(), all_adjacent.end(), cell->getPosition());

			if (is_found != all_adjacent.end())
			{
				all_adjacent.erase(is_found);
			}

			count_adj++;
		}

		if (!all_adjacent.empty())
			return *(all_adjacent.begin());
		return position_;
	}
};




