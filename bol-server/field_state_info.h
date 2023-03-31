#pragma once
#include "field_state.h"

class FieldStateInfo final
{
public:
	void init(std::shared_ptr<GameConfig> config)
	{
		config_ = config;
		game_state_->initConfig(config_);
	}

	void update();

	const std::vector<CellInfo>& getCellInfo() const { return gui_field_state_; }

	void reset();

private:
	std::vector<CellInfo> gui_field_state_;
	std::shared_ptr<FieldState> game_state_ = std::make_shared<FieldState>();
	std::shared_ptr<GameConfig> config_ = std::make_shared<GameConfig>();
};

