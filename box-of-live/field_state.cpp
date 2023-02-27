#include "field_state.h"
#include "bacterium.h"
#include "grass.h"
#include "thread"

void FieldState::addColonyBacterium(unsigned int max_count)
{
	data_cell_.insert({ {0,0}, std::make_shared < Bacterium > (0) });
	//srand(time(NULL));
	//for (unsigned int id_bacterium = 0; id_bacterium < NUMBER_BACTERIAL_COLONIES; id_bacterium++)
	//{
	//	auto new_bacterium = std::make_shared<Bacterium>(id_bacterium);
	//	new_bacterium->setEnergy(5);
	//	new_bacterium->setSpeed(2);

	//	const unsigned int colony_size = getRandomUInt(10, max_count);
	//	std::vector<std::shared_ptr<Bacterium>> colony(colony_size, new_bacterium);

	//	Position base_bac_position = getRandomPosition();
	//	/*while(data_cell_.find(base_bac_position) != data_cell_.end())
	//	{
	//		base_bac_position = getRandomPosition();
	//	}*/
	//	int min_neigh = 4;
	//	int count_neigh = 0;
	//	data_cell_.insert({ base_bac_position, std::move(new_bacterium) });

	//	auto pos_neigh = base_bac_position.getRandomDirection();

	//	for(auto bac:colony)
	//	{
	//		while (data_cell_.find(pos_neigh) != data_cell_.end())
	//		{
	//			pos_neigh = base_bac_position.getRandomDirection();
	//		}

	//		data_cell_.insert({ pos_neigh, std::move(bac) });
	//		count_neigh++;
	//		if(count_neigh == min_neigh)
	//		{
	//			base_bac_position = pos_neigh;
	//			count_neigh = 0;
	//		}
	//	}
	//}

	//srand(time(NULL));
	//for (unsigned int id_bac = 0; id_bac < NUMBER_BACTERIAL_COLONIES; id_bac++) {
	//	int count_bacterium = 0;
	//	Position  base_bac_position;
	//	while (count_bacterium <= 1)
	//	{//позиция первой бактерии для колонии
	//		 base_bac_position = getRandomPosition();
	//		if (data_cell_.find(base_bac_position) != data_cell_.end())
	//			continue;
	//		auto new_bacterium = std::make_shared<Bacterium>(id_bac);
	//		new_bacterium->setEnergy(5);
	//		new_bacterium->setSpeed(2);
	//		data_cell_.insert({ base_bac_position , std::move(new_bacterium) });
	//		++count_bacterium;
	//	}
	//	//строим колонию
	//	unsigned int colony_size = getRandomUInt(20, max_count); //кол-во бактерий(клеток, которые нужно заполнить) (рандом от 20 до 30)

	//	//Position base = pos_base_bac; // Позиция начала колонии 
	//	unsigned int min_neigh = 3; //  Минимальное кол-во соседей у одной бактерии, кол-во клеток, которые нужно заполнить
	//	unsigned int curr_neigh = 0;
	//	while(count_bacterium <= colony_size)
	//	{
	//		auto neigh_pos = base_bac_position.getRandomDirection();
	//	/*	if (data_cell_.find(neigh_pos) != data_cell_.end())
	//			continue;*/
	//		auto new_bacterium = std::make_shared<Bacterium>(id_bac);
	//		new_bacterium->setEnergy(5);
	//		new_bacterium->setSpeed(2);
	//		data_cell_.insert({ neigh_pos,std::move(new_bacterium) });
	//		++count_bacterium;
	//		++curr_neigh;
	//		if(curr_neigh == min_neigh)
	//		{
	//			base_bac_position = neigh_pos;
	//			curr_neigh = 0;
	//		}
	//	}
	//}
}

void FieldState::addGrass(unsigned int amount_grass)
{
	/*int count = 0;
	srand(time(NULL));
	while (count <= amount_grass)
	{
		Position new_position(getRandomPosition());
		if (data_cell_.find(new_position) == data_cell_.end())
		{
			data_cell_.insert({ new_position, std::make_shared<EmptyCell>() });
		}
		if (data_cell_[new_position]->getCellType() == TypeCell::EMPTY)
		{
			data_cell_[new_position] = std::make_shared<Grass>();
			++count;
		}
	}*/

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
