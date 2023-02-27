#include "field_state.h"
#include "bacterium.h"
#include "grass.h"
#include "thread"

void FieldState::addBacterium(unsigned int max_count)
{
	srand(time(NULL));
	for (unsigned int id_bac = 0; id_bac < NUMBER_BACTERIAL_COLONIES; id_bac++) {
		Position pos_base_bac;
		while (true)
		{//позиция первой бактерии для колонии
			auto rand_pos(getRandomPosition());
			pos_base_bac = rand_pos;
			if (data_cell_.find(pos_base_bac) != data_cell_.end())
				continue;
			auto new_bacterium = std::make_shared<Bacterium>(id_bac);
			new_bacterium->setEnergy(5);
			new_bacterium->setSpeed(2);
			data_cell_[pos_base_bac] = std::move(new_bacterium);
			
			break;
		}
		//строим колонию
		unsigned int rcount = getRandomUInt(15, max_count); //кол-во бактерий(клеток, которые нужно заполнить) (рандом от 15 до 30)

		Position base = pos_base_bac; // Позиция начала колонии 
		unsigned int min_neigh = 4; //  Минимальное кол-во соседей у одной бактерии, кол-во клеток, которые нужно заполнить

		for (unsigned int i = 0; i <= rcount;) {
			auto neigh_pos = base.getRandomDirection();

			auto new_bacterium = std::make_shared<Bacterium>(id_bac);
			new_bacterium->setEnergy(5);
			new_bacterium->setSpeed(2);
			data_cell_.insert({ neigh_pos,std::move(new_bacterium) });// Пока у базовой клетки не будет 4х соседих бактерий, не меняем базовую клетку

			if (i == min_neigh) {
				base = neigh_pos; // теперь базовой будет последняя рандомная соседняя клетка
			}

			i++;
		}
	}
}

void FieldState::addGrass(unsigned int amount_grass)
{
	int count = 0;
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

	if(timer_bacterium_.timedOut())
	{
		for (const auto&[pos, cell] : data_cell_)
		{
			switch (auto type = cell->getCellType(); type)
			{
			case TypeCell::GRASS:
				break;
			case TypeCell::EMPTY:
				break;
			case TypeCell::BACTERIUM:
				{
				Cell& curr_cell = *cell;
				auto curr_bacterium = dynamic_cast<Bacterium&>(curr_cell);
					if(curr_bacterium.canMove())
					{
						auto new_pos = pos.getRandomDirection();
						if (data_cell_.find(new_pos) == data_cell_.end())
							data_cell_.insert({ new_pos, std::make_shared<EmptyCell>() });
						auto new_pos_cell_type = cell->getCellType();
						
						if(new_pos_cell_type == TypeCell::BACTERIUM)
						{
							Cell& new_pos_cell = *data_cell_[new_pos];
							auto new_pos_cell_bacterium = dynamic_cast<Bacterium&>(new_pos_cell);
							if(curr_bacterium.getIdType() != new_pos_cell_bacterium.getIdType())
							{
								////////////////////////////////////////////////////////////
								data_cell_[new_pos] = cell;
								data_cell_[pos] = std::make_shared<EmptyCell>();
							}
						}
						else if(new_pos_cell_type == TypeCell::GRASS)
						{
							
						}
						else if(new_pos_cell_type == TypeCell::EMPTY)
						{
							
						}
					}
				}
			}
		}
	}
	
	/*for(const auto& [pos, cell] : data_cell_)
	{
		switch (auto type = cell->getCellType(); type)
		{
		case TypeCell::GRASS:
	
			break;
		case TypeCell::BACTERIUM:
		{
			auto new_pos = pos.getRandomDirection();
			if (data_cell_.find(new_pos) == data_cell_.end())
				data_cell_.insert({ new_pos, std::make_shared<EmptyCell>() });

			Cell& c = *cell;
			auto bacterium = dynamic_cast<Bacterium&>(c);
			auto now = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsed_seconds = now - bacterium.getLastActionTime();
			if (elapsed_seconds.count() <= 2)
			{
				break;
			}
			if (bacterium.canMove()) {
				switch (auto dir_type = data_cell_[new_pos]->getCellType(); dir_type)
				{
				case TypeCell::BACTERIUM:
				{
					Cell& dir_c = *cell;
					auto id_dir_bacterium = dynamic_cast<Bacterium&>(dir_c).getIdType();
					if (bacterium.getIdType() != id_dir_bacterium)
					{
						data_cell_[new_pos] = cell;
						data_cell_[pos] = std::make_shared<EmptyCell>();
					}
					break;
				}
				case TypeCell::GRASS:
				{
					data_cell_[new_pos] = cell;
					data_cell_[pos] = std::make_shared<EmptyCell>();
					break;
				}
				case TypeCell::EMPTY:
				{
					data_cell_[new_pos] = cell;
					data_cell_[pos] = std::make_shared<EmptyCell>();
					break;
				}
				}
			}
			break;
		}
		case TypeCell::EMPTY:
			break;
		}
	}*/
}
