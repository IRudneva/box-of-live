#include "field_state.h"
#include "bacterium.h"
#include "grass.h"
#include "thread"

void FieldState::addColonyBacterium(unsigned int max_count)
{
	srand(time(NULL));
	for (unsigned int id_bacterium = 0; id_bacterium < NUMBER_BACTERIAL_COLONIES; id_bacterium++)
	{
		auto base_bacterium = std::make_shared<Bacterium>(id_bacterium); // создаем базовую бактерию
		Position base_bac_position = getRandomPosition(); // получаем рандомную позицию на поле
		while (data_cell_.find(base_bac_position) != data_cell_.end()) 
		{
			base_bac_position = getRandomPosition();
		}
		base_bacterium->setPosition(base_bac_position);
		data_cell_.insert({ base_bac_position, base_bacterium });
		
		int count_bacterium = 1; // текущее кол-во бактерий в колонии

		const int max_adjacent = 3; // максимальное кол-во соседей для одной клетки
		int curr_adjacent = 0; // текущее кол-во соседей

		const auto colony_size = getRandomUInt(10, max_count);

		while (count_bacterium < colony_size) {

			auto adjacent_position = base_bac_position.getRandomDirection(); //получаем рандомную соседнюю клетку
			if (data_cell_.find(adjacent_position) != data_cell_.end())// если такая позиция есть, получаем другую позицию
				continue;

			auto adjacent = std::make_shared<Bacterium>(id_bacterium);
			adjacent->setPosition(adjacent_position);
			auto last_bacterium = data_cell_.insert({ adjacent_position, adjacent });
			auto last_pos = last_bacterium.first->first;
			count_bacterium++; //увеличиваем счётчик бактерий
			curr_adjacent++; //увеличиваем счётчик соседей

			if (curr_adjacent == max_adjacent) // когда соседей станет max_adjacent, базовой позицией станет последняя успешно добавленная
			{
				base_bac_position = last_pos;
				curr_adjacent = 0;
			}
		}
	}
}


void FieldState::addGrass(unsigned int amount_grass)
{
	int count = 0;
	srand(time(NULL));
	while (count <= amount_grass)
	{
		Position new_position = getRandomPosition();
		if (data_cell_.find(new_position) != data_cell_.end())
		{
			new_position = getRandomPosition();
		}

		data_cell_[new_position] = std::make_shared<Grass>();
		++count;
	}
}

void FieldState::update()
{
	if (data_cell_.empty())
		return;
	
	if (timer_grass_.timedOut())
	{
		addGrass(50);
	}

	//if (timer_bacterium_.timedOut())
	//{
	//	for (const auto&[pos, cell] : data_cell_) // обычный фор или вайл
	//	{
	//		switch (auto type = cell->getCellType(); type)
	//		{
	//		case TypeCell::GRASS:
	//			break;
	//		case TypeCell::EMPTY:
	//			break;
	//		case TypeCell::BACTERIUM:
	//		{
	//			Cell& curr_cell = *cell;
	//			auto curr_bacterium = dynamic_cast<Bacterium&>(curr_cell);
	//			if (curr_bacterium.canMove())
	//			{
	//				auto new_pos = pos.getRandomDirection();
	//				if (data_cell_.find(new_pos) == data_cell_.end())
	//					data_cell_.insert({ new_pos, std::make_shared<EmptyCell>() });
	//				auto new_pos_cell_type = data_cell_[new_pos]->getCellType();

	//				if (new_pos_cell_type == TypeCell::BACTERIUM)
	//				{
	//					Cell& new_pos_cell = *data_cell_[new_pos];
	//					auto new_pos_cell_bacterium = dynamic_cast<Bacterium&>(new_pos_cell);
	//					if (curr_bacterium.getIdType() != new_pos_cell_bacterium.getIdType())
	//					{
	//						if (curr_bacterium.getEnergy() > new_pos_cell_bacterium.getEnergy())
	//						{
	//							curr_bacterium.setEnergy(new_pos_cell_bacterium.getEnergy() / 2);
	//							data_cell_[new_pos] = cell;
	//							data_cell_[pos] = std::make_shared<EmptyCell>();
	//						}
	//						///////////////////////////////////////////////////////////
	//					}
	//				}
	//				else if (new_pos_cell_type == TypeCell::GRASS)
	//				{
	//					curr_bacterium.setEnergy(curr_bacterium.getEnergy() + 5);
	//					auto curr_energy = curr_bacterium.getEnergy();
	//					if (curr_energy > 6)
	//					{
	//						curr_bacterium.setEnergy(curr_energy / 2);
	//						curr_energy = curr_bacterium.getEnergy();
	//						data_cell_[new_pos] = cell;
	//						auto chaild_bacterium = std::make_shared<Bacterium>(curr_bacterium.getIdType());
	//						chaild_bacterium->setEnergy(curr_energy);
	//						data_cell_[pos] = chaild_bacterium;

	//					}
	//					else
	//					{
	//						data_cell_[new_pos] = cell;
	//						data_cell_[pos] = std::make_shared<EmptyCell>();
	//					}
	//				}
	//				else if (new_pos_cell_type == TypeCell::EMPTY)
	//				{
	//					data_cell_[new_pos] = cell;
	//					data_cell_[pos] = std::make_shared<EmptyCell>();
	//				}
	//			}
	//		}
	//		}
	//	}
	//}
}
