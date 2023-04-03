#pragma once
#include "field_state.h"

class FieldStateInfo final
{
public:
	void init(std::shared_ptr<GameConfig> config);

	void update();

	const std::vector<GrassInfo>& getCellInfo() const { return cells_state_; }

	const std::vector<BacteriumInfo>& getBacteriumInfo() const { return bacterium_state_; }

	void reset();

private:
	std::vector<GrassInfo> cells_state_;
	std::vector<BacteriumInfo> bacterium_state_;
	std::shared_ptr<FieldState> game_state_ = std::make_shared<FieldState>();
	std::shared_ptr<GameConfig> config_ = std::make_shared<GameConfig>();

	void fillVectorsInfo();
};

