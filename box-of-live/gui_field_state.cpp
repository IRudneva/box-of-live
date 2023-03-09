#include "gui_field_state.h"
#include "bacterium.h"

void GuiFieldState::update()
{
	gui_field_state_.clear();
	game_state_->update();
	const auto state = game_state_->getData();

	for (const auto&[id, cell] : state)
	{
		const auto[x, y] = cell->getPosition();
		BacteriumInfo inf_bac;
		if (cell->getCellType() == TypeCell::BACTERIUM)
		{
			Cell& a = *cell;
			auto bacterium = dynamic_cast<Bacterium&>(a);
			inf_bac = { bacterium.getIdType(), bacterium.getEnergy() };
		}
		gui_field_state_.push_back({ cell->getCellType(), x,y, inf_bac });
	}
}

void GuiFieldState::reset()
{
	game_state_->restart();
	gui_field_state_.clear();
}
