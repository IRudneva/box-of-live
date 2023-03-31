#include "pch_server.h"
#include "field_state_info.h"
#include "bacterium.h"

void FieldStateInfo::update()
{
	gui_field_state_.clear();
	game_state_->update();
	const auto state = game_state_->getData();

	for (const auto&[id, cell] : state)
	{
		const auto[x, y] = cell->getPosition();
		std::shared_ptr<BacteriumInfo> inf_bac;
		if (cell->getCellType() == TypeCell::BACTERIUM)
		{
			Cell& a = *cell;
			auto bacterium = dynamic_cast<Bacterium&>(a);
			inf_bac = std::make_shared<BacteriumInfo>(bacterium.getIdType(), bacterium.getEnergy());
		}
		gui_field_state_.emplace_back(cell->getCellType(), x,y, inf_bac);
	}
}

void FieldStateInfo::reset()
{
	game_state_->restart();
	gui_field_state_.clear();
}
