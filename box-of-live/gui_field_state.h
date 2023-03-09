#pragma once
#include "field_state.h"

class GuiFieldState final
{
public:
	void init(std::shared_ptr<GameConfig> config) { game_state_->init(config); }

	void update();

	const std::vector<CellInfo>& getCellInfo() const { return gui_field_state_; }

	void reset();

private:
	std::vector<CellInfo> gui_field_state_;
	std::shared_ptr<FieldState> game_state_ =std::make_shared<FieldState>();
};


