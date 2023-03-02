#include "field_state.h"
#include "bacterium.h"
#include "grass.h"
#include "thread"

void FieldState::addColonyBacterium(int max_count)
{
	srand(time(NULL));
	for (unsigned int id_bacterium = 0; id_bacterium < NUMBER_BACTERIAL_COLONIES; id_bacterium++)
	{
		auto base_bacterium = std::make_shared<Bacterium>(id_bacterium); // создаем базовую бактерию

		Position base_bac_position = getRandomEmptyPosition(); // находим для нее пустую клетку

		base_bacterium->setPosition(base_bac_position); // присваиваем бактерии эту позицию

		cells_.insert({ base_bacterium->getIdCell(), base_bacterium });


		//
		//
		//int count_bacterium = 1; // текущее кол-во бактерий в колонии

		//const int max_adjacent = 3; // максимальное кол-во соседей для одной клетки
		//int curr_adjacent = 0; // текущее кол-во соседей

		//const auto colony_size = getRandomInt(10, max_count);

		//while (count_bacterium < colony_size) {

		//	auto adjacent_position = getRandomEmptyAdjacent(base_bac_position); // получаем позицию нового соседа
		//	if(adjacent_position == base_bac_position) // нет свободных соседний позиций
		//	{
		//		base_bac_position = base_bac_position.getRandomDirection(); // базовой становится любой сосед
		//		continue; // пока не найдём свобоную клетку
		//	}


		//	
		//	auto adjacent = std::make_shared<Bacterium>(id_bacterium); // создаём бактерию соседа

		//	adjacent->setPosition(adjacent_position); // присваиваем бактерии найденную позицию
		//	
		//	auto last_bacterium = cells_.insert({ adjacent->getIdCell(), adjacent });


		//	

		//	count_bacterium++; //увеличиваем счётчик бактерий
		//	curr_adjacent++; //увеличиваем счётчик соседей

		//	if (curr_adjacent == max_adjacent) // когда соседей станет max_adjacent, базовой позицией станет последняя успешно добавленная
		//	{
		//		auto last_pos = last_bacterium.first->second->getPosition();
		//		base_bac_position = last_pos;
		//		curr_adjacent = 0;
		//	}
		//}
	}
}


void FieldState::addGrass(int amount_grass)
{
	int count = 0;
	srand(time(NULL));
	while (count < amount_grass)
	{
		Position new_position = getRandomEmptyPosition();
		auto new_grass = std::make_shared<Grass>();
		new_grass->setPosition(new_position);
		cells_.insert({ new_grass->getIdCell(), new_grass });
		++count;
	}
}

void FieldState::update()
{
	if (cells_.empty())
		return;

	if (timer_grass_.timedOut())
		addGrass(50);
	std::vector<std::shared_ptr<Cell>> child_bacterium;
	
	for (const auto&[id, cell] : cells_)
	{
		if (cell->isReadyUpdate())
		{
			cell->update(cells_);
			if (cell->canClone())
			{
				auto child = cell->clone(cells_);
				if (child != nullptr)
					child_bacterium.push_back(child);
			}
			
		}
	}
	
	for (const auto& ch : child_bacterium)
	{
		cells_.insert({ ch->getIdCell(), ch });
	}
	
	for (const auto&[id, cell] : cells_)
	{
		if (cell != nullptr) {
			if (auto t = cell->getEraseId(); t.has_value())
			{
				cells_[t.value()] = nullptr;
			}
		}
	}

	for (auto it = cells_.begin(); it != cells_.end(); )
	{

		if (it->second == nullptr)
		{
			it = cells_.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void FieldState::restart()
{
	cells_.clear();
	//data_cell_.clear();
	IdCell::reset();
	addColonyBacterium(20);
	addGrass(300);
}

Position FieldState::getRandomEmptyPosition() const
{
	Position position = getRandomPosition(); // получаем рандомную позицию на поле
	for(const auto& [id, cell] : cells_)
	{
		if(cell->getPosition()==position)
		{
			position = getRandomPosition();
		}
	}
	
	return position;
}

Position FieldState::getRandomEmptyAdjacent(Position position) const
{
	int count = 0;
	auto adjacent_position = position.getRandomDirection(); //получаем рандомную соседнюю клетку
	for(const auto& [id, cell] : cells_)
	{
		if (count == position.getCountAdjacent()) //если кол-во попыток найти пустую соседнюю клетку закончилось, выходим из цикла 
			return position; // возвращаем текущую позицию
		if (cell->getPosition() == adjacent_position)
		{
			adjacent_position = getRandomPosition();
			count++;
		}
	}
	return adjacent_position;
}