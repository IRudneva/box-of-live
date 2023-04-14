#include "pch_server.h"
#include "field_state_info.h"

#include <utility>
#include "bacterium.h"

void FieldStateInfo::init(std::shared_ptr<GameConfig> config)
{
	config_ = std::move(config);
	game_state_->initConfig(config_);
}

void FieldStateInfo::update()
{
	game_state_->update();
	fillVectorsInfo(/*game_state_->getDeltaGameState()*/);
}

void FieldStateInfo::reset()
{
	game_state_->restart();
	fillVectorsInfo(/*game_state_->getDeltaGameState()*/);
}

void FieldStateInfo::fillVectorsInfo(/*const DeltaGameState& delta*/)
{
	cells_state_.clear();
	bacterium_state_.clear();
	const auto state = game_state_->getData();

	/*for (const auto& pos : delta.deleted_cells)
	{
		deleted_position_.emplace_back(pos.x, pos.y);
	}
*/
	/*for(const auto& pos : delta.added_cells)
	{
		if(auto cell = game_state_->getCellInPosition(pos); cell!=nullptr)
		{
			if(cell->getCellType() == TypeCell::BACTERIUM)
			{
				Cell& a = *cell;
				auto bacterium = dynamic_cast<Bacterium&>(a);
				BacteriumInfo inf_bac(pos.x, pos.y, bacterium.getIdType(), bacterium.getEnergy());
				bacterium_state_.emplace_back(inf_bac);
			}
			else if(cell->getCellType() == TypeCell::GRASS)
			{
				GrassInfo inf_grass(pos.x, pos.y);
				cells_state_.emplace_back(inf_grass);
			}
		}
	}*/

	for (const auto&[id, cell] : state)
	{
		const auto[x, y] = cell->getPosition();
		if (cell->getCellType() == TypeCell::BACTERIUM)
		{
			Cell& a = *cell;
			auto bacterium = dynamic_cast<Bacterium&>(a);
			BacteriumInfo inf_bac(x, y, bacterium.getIdType(), bacterium.getEnergy());
			bacterium_state_.emplace_back(inf_bac);
		}
		else
		{
			GrassInfo inf_grass(x, y);
			cells_state_.emplace_back(inf_grass);
		}
	}
}