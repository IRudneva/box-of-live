#pragma once
#include <memory>
#include <optional>
#include <TGUI/TGUI.hpp>
#include "cell.h"

constexpr static int ENERGY_ACTION_COST = 1;
constexpr static int ENERGY_BASE = 5;
constexpr static int ENERGY_TO_CLONE = 4;
constexpr static int UPDATE_TIME = 2;
constexpr static int ENERGY_GRASS = 3;


class Bacterium: public Cell {
public:
	Bacterium(unsigned int id_type)
		:id_type_(id_type)
	{
		setCellType(TypeCell::BACTERIUM);
	}

	void setPosition(Position pos) { position_ = pos; }

	Position getPosition() const { return  position_; }
	
	void setIdType(unsigned int id) { id_type_ = id; }

	unsigned int getIdType() const { return id_type_; }

	void setEnergy(unsigned int energy) { energy_base_ = energy; }

	unsigned int getEnergy() const { return energy_base_; }

	void setSpeed(unsigned int speed) { speed_ = speed; }

	unsigned int getSpeed() const { return speed_; }
	   	
	std::optional<UpdateState> update(const std::unordered_map<Position,std::shared_ptr<Cell>,PositionHasher>& data_cell) override
	{
		spendEnergy(ENERGY_ACTION_COST); //тратит энергию 
		if (!canMove()) // если энергии недостаточно для перемещения - стоит на месте
			return std::nullopt;
		// если енергии хватает для перемещения
		auto update_position = tryMove(data_cell);
		if (update_position.has_value())
		{
			if (canClone())
				clone(update_position.value(), data_cell);
			
			return update_position;
		}
		return std::nullopt;
	}

private:
	unsigned int id_type_ = 0;
	Position position_;
	unsigned int energy_base_ = ENERGY_BASE;
	unsigned int speed_ = UPDATE_TIME;
	std::chrono::time_point<std::chrono::steady_clock> last_action_time_;


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

	void clone(UpdateState state, const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell)
	{
		auto all_adjacent = state.new_position.getAllAdjacentPosition();
		for (auto adj : all_adjacent)
		{
			if (data_cell.find(adj) == data_cell.end())
				state.chaild_position = adj;
		}
		state.chaild_position = std::nullopt;
	}

	/*пытается переместиться в случайную точку рядом с собой, если не получается - стоит на месте
		не может перемещаться на точку где есть бактерия такого же типа
		не может перемещаться на точку если у другой бактерии энергии больше
		приоритет выбора точек : бактерия другого типа, трава, пустая клетка*/

	std::optional<UpdateState> tryMove(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell)
	{
		std::optional<UpdateState> result;

		auto all_adjacent = position_.getAllAdjacentPosition();
		
		// логика энергиии!!
		
		auto another_bacterium_pos = tryFindBacteriumAnotherType(data_cell);
		if(another_bacterium_pos.has_value())
		{
			if(another_bacterium_pos.value() == position_)
			{
				return std::nullopt; // у бактерии все соседи ее типа
			}
			
			Cell& another_cell = *data_cell.at(another_bacterium_pos.value());
			const auto another_bacterium = dynamic_cast<Bacterium&>(another_cell);
			if(energy_base_ > another_bacterium.getEnergy())
			{
				result.emplace(UpdateState{ position_, another_bacterium_pos.value(), std::nullopt });
				spendEnergy(another_bacterium.getEnergy() * 0.5);
				return result;
				// перемещаемся на клетку другой бактерии и зануляем текущую позицию
			}
		}// если у другой бактерии энергии больше - ищем другую позицию - траву

		auto grass_pos = tryFindGrass(data_cell);

		if(grass_pos.has_value())
		{
			result.emplace(UpdateState{ position_, grass_pos.value(), std::nullopt });
			increaseEnergy(ENERGY_GRASS);
			return result;
			// перемещаемся на клетку с травой и зануляем текущую позицию
		}
		
		srand(time(NULL));
		auto empty_pos = position_.getRandomDirection();

		while (data_cell.find(empty_pos) != data_cell.end())
		{
			empty_pos = position_.getRandomDirection();
		}
		result.emplace(UpdateState{ position_, empty_pos, std::nullopt });
		return result;
		// перемещаемся на свободную позицию

	}

	std::optional<Position> tryFindBacteriumAnotherType(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell)
	{
		int count_bacterium = 0;
		auto all_adjacent = position_.getAllAdjacentPosition();
		for (const auto& adj_pos : all_adjacent) 
		{
			if(data_cell.find(adj_pos) != data_cell.end() && data_cell.at(adj_pos)->getCellType() == TypeCell::BACTERIUM)
			{
				count_bacterium++;
				Cell& adj_cell = *data_cell.at(adj_pos);
				const auto adj_bacterium = dynamic_cast<Bacterium&>(adj_cell);
				if (adj_bacterium.getIdType() != id_type_) // если хоть одна бактерия другого типа
				{
					return adj_pos; // возвращаем позицию этой бактерии 
				}
			}
		}
		if (count_bacterium == all_adjacent.size())
			return position_;
		
		return std::nullopt; // если рядом нет бактерии другого типа возвращаемся
	}

	std::optional<Position> tryFindGrass(const std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell)
	{
		auto all_adjacent = position_.getAllAdjacentPosition();
		for (const auto& adj_pos : all_adjacent)
		{
			if (data_cell.find(adj_pos) != data_cell.end() && data_cell.at(adj_pos)->getCellType() == TypeCell::GRASS)
			{
				return  adj_pos;
			}
		}
		return std::nullopt;
	}
};




