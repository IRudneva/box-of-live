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
			//if (data_cell_.find(adjacent_position) != data_cell_.end())// если такая позиция есть, получаем другую позицию
			//	continue;

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
	while (count < amount_grass)
	{
		Position new_position = getRandomPosition();
		if (data_cell_.find(new_position) != data_cell_.end())
			continue;

		data_cell_[new_position] = std::make_shared<Grass>();
		++count;
	}
}

void FieldState::update()
{
	if (data_cell_.empty())
		return;
	
	if (timer_grass_.timedOut())
		addGrass(50);
	// ПРОВЕРИТЬ, ПРИШЛО ЛИ ВРЕМЯ БАКТЕРИИ ПЕРЕМЕСТИТЬСЯ, ЕСЛИ ДА, ПОЛОЖИТЬ ЕЕ В ДРГОЙ КОНТЕЙНЕР И ОБНОВИТЬ
	std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> update_positions;

	
	for(auto it=data_cell_.begin(); it != data_cell_.end();) //Не работает, падает с ошибкой выхода итератора
	{
		if(it->second->getCellType() == TypeCell::BACTERIUM)
		{
			// тут проверить время
			if (auto up = it->second->update(data_cell_); up.has_value())
			{
				it = data_cell_.erase(data_cell_.find(up.value().old_position));
				update_positions.insert({ up.value().new_position, it->second });
				
				if (up.value().chaild_position.has_value())
					update_positions.insert({ up.value().chaild_position.value(), it->second });
			}
			else 
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}
	for(auto&[pos, cell] : update_positions)
	{
		data_cell_.insert({pos, cell});
	}
}
