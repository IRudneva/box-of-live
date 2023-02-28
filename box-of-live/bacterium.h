#pragma once
#include <memory>
#include <optional>
#include <TGUI/TGUI.hpp>
#include "cell.h"

constexpr static int ENERGY_ACTION_COST = 1;
constexpr static int ENERGY_BASE = 5;
constexpr static int ENERGY_TO_CLONE = 3;
constexpr static int UPDATE_TIME = 2;

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
	   	
	void update(std::unordered_map<Position,std::shared_ptr<Cell>,PositionHasher> data_cell) override
	{
		spendEnergy(); //тратит энергию 
		if (!canMove()) // если энергии недостаточно для перемещения - стоит на месте
			return;
		// если енергии хватает для перемещения
		if(tryMove(data_cell))
		{
			
		}
		/////////////////////////////////////////////////////////////////////////////////////
		//ПЕРЕНЕСТИ ВСЮ ЛОГИКУ ОБНОВЛЕНИЯ КЛЕТКИ БАКТЕРИИ ИЗ СОСТОЯНИЯ ПОЛЯ
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

	void spendEnergy()
	{
		if (energy_base_ != 0)
			--energy_base_;
	}

	/*пытается переместиться в случайную точку рядом с собой, если не получается - стоит на месте
		не может перемещаться на точку где есть бактерия такого же типа
		не может перемещаться на точку если у другой бактерии энергии больше
		приоритет выбора точек : бактерия другого типа, трава, пустая клетка*/

	bool tryMove(std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell)
	{
		srand(time(NULL));

		Position result_position;

		auto all_adjacent = position_.getAllAdjacentPosition();

		auto r = tryFindBacteriumAnotherType(data_cell);

		for(const auto& adj:all_adjacent)
		{
			
		}
		
		auto new_pos = position_.getRandomDirection();

	}

	std::optional<Position> tryFindBacteriumAnotherType(std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell)
	{
		int count_bacterium = 0;
		auto all_adjacent = position_.getAllAdjacentPosition();
		for (const auto& adj_pos : all_adjacent) 
		{
			if(data_cell[adj_pos]->getCellType() == TypeCell::BACTERIUM)
			{
				count_bacterium++;
				Cell& adj_cell = *data_cell[adj_pos];
				const auto adj_bacterium = dynamic_cast<Bacterium&>(adj_cell);
				if (adj_bacterium.getIdType() != id_type_) // если хоть одна бактерия другого типа
				{
					return adj_pos; // возвращаем позицию этой бактерии 
				}
				//////////////////////////////////////////
				//
			}
		}
	}

	//bool checkAllAdjacentOfTheSameType(std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher>& data_cell) 
	//{
	//	auto all_adjacent = position_.getAllAdjacentPosition();
	//	auto 

	//	if (std::all_of(all_adjacent.begin(), all_adjacent.end(), // если все соседи есть в data cell и все они бактерии 
	//		[&data_cell](const Position pos)
	//	{
	//		return data_cell.find(pos) != data_cell.end()
	//			&& data_cell[pos]->getCellType() == TypeCell::BACTERIUM;
	//	}))
	//	{
	//		for (const auto& adj_pos : all_adjacent) // проверяем, все ли соседние бактерии такого же типа
	//		{
	//			Cell& adj_cell = *data_cell[adj_pos];
	//			const auto adj_bacterium = dynamic_cast<Bacterium&>(adj_cell);
	//			if (adj_bacterium.getIdType() != id_type_) // если хоть одна бактерия другого типа
	//				return false;
	//		}
	//		return true;
	//	}
	//}
};




