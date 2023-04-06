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
	fillVectorsInfo();
}

void FieldStateInfo::reset()
{
	game_state_->restart();
	fillVectorsInfo();
}

void FieldStateInfo::fillVectorsInfo()
{
	cells_state_.clear();
	bacterium_state_.clear();
	const auto state = game_state_->getData();
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