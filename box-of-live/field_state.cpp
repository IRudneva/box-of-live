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

		//base_bacterium->setIdCell(data_bacterium_.size());

		Position base_bac_position = getRandomEmptyPosition(); // находим для нее пустую клетку

		base_bacterium->setPosition(base_bac_position); // присваиваем бактерии эту позицию

		cells_.insert({ base_bacterium->getIdCell(), base_bacterium });

		//data_bacterium_.insert(base_bacterium); // добавляем в хранилище бактерий

		data_cell_.insert({ base_bac_position, base_bacterium }); // добавляем на поле
		
		int count_bacterium = 1; // текущее кол-во бактерий в колонии

		const int max_adjacent = 3; // максимальное кол-во соседей для одной клетки
		int curr_adjacent = 0; // текущее кол-во соседей

		const auto colony_size = getRandomUInt(10, max_count);

		while (count_bacterium < colony_size) {

			auto adjacent_position = getRandomEmptyAdjacent(base_bac_position); // получаем позицию нового соседа
			if(adjacent_position == base_bac_position) // нет свободных соседний позиций
			{
				base_bac_position = base_bac_position.getRandomDirection(); // базовой становится любой сосед
				continue; // пока не найдём свобоную клетку
			}

			auto adjacent = std::make_shared<Bacterium>(id_bacterium); // создаём бактерию соседа

			adjacent->setPosition(adjacent_position); // присваиваем бактерии найденную позицию

			cells_.insert({ adjacent->getIdCell(), adjacent });
			//data_bacterium_.insert(base_bacterium); // добавляем в хранилище бактерий

			auto last_bacterium = data_cell_.insert({ adjacent_position, adjacent }).first; // добавляем на поле и возвращаем итератор на добавленную бактерию

			count_bacterium++; //увеличиваем счётчик бактерий
			curr_adjacent++; //увеличиваем счётчик соседей

			if (curr_adjacent == max_adjacent) // когда соседей станет max_adjacent, базовой позицией станет последняя успешно добавленная
			{
				auto last_pos = last_bacterium->first;
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
		Position new_position = getRandomEmptyPosition();
		auto new_grass = std::make_shared<Grass>();
		data_cell_.insert({ new_position, new_grass });
		cells_.insert({ new_grass->getIdCell(), new_grass });
		++count;
	}
}

void FieldState::update()
{
	if (data_cell_.empty())
		return;
	/*
	if (timer_grass_.timedOut())
		addGrass(50);*/

	// ПРОВЕРИТЬ, ПРИШЛО ЛИ ВРЕМЯ БАКТЕРИИ ПЕРЕМЕСТИТЬСЯ, ЕСЛИ ДА, ПОЛОЖИТЬ ЕЕ В ДРГОЙ КОНТЕЙНЕР И ОБНОВИТЬ
	//std::unordered_map<Position, std::shared_ptr<Cell>, PositionHasher> update_positions;
	//std::vector<Position> erase_position;

	//
	//for(auto it=data_cell_.begin(); it != data_cell_.end(); ++it) //Не работает, падает с ошибкой выхода итератора
	//{
	//	if (it->second->getCellType() == TypeCell::BACTERIUM)
	//	{
	//		// тут проверить время
	//		if (auto up = it->second->update(data_cell_); up.has_value())
	//		{
	//			update_positions.insert({ up.value().new_position, it->second });

	//			if (up.value().chaild_position.has_value())
	//				update_positions.insert({ up.value().chaild_position.value(), it->second });

	//			erase_position.push_back(up.value().old_position);
	//		//	it = data_cell_.erase(data_cell_.find(up.value().old_position));
	//		}
	//	}
	//}


	//for(auto&[pos, cell] : update_positions)
	//{
	//	data_cell_.insert({pos, cell});
	//}

	//for (auto p : erase_position)
	//{
	//	auto er = data_cell_.find(p);
	//	if(er!=data_cell_.end())
	//		data_cell_.erase(er);
	//}

}

void FieldState::restart()
{
	data_cell_.clear();
	IdCell::reset();
	addColonyBacterium(20);
	addGrass(200);
}

Position FieldState::getRandomEmptyPosition() const
{
	Position position = getRandomPosition(); // получаем рандомную позицию на поле
	while (data_cell_.find(position) != data_cell_.end())
			position = getRandomPosition();
	return position;
}

Position FieldState::getRandomEmptyAdjacent(Position position) const
{
	int count = 0;
	auto adjacent_position = position.getRandomDirection(); //получаем рандомную соседнюю клетку
	while (data_cell_.find(adjacent_position) != data_cell_.end())// если такая позиция есть, получаем другую позицию
	{
		if (count == position.getCountAdjacent()) //если кол-во попыток найти пустую соседнюю клетку закончилось, выходим из цикла 
			return position; // возвращаем текущую позицию
		adjacent_position = position.getRandomDirection();
		count++;
	}
	return adjacent_position;
}