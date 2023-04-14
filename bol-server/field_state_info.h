#pragma once
#include "field_state.h"

class FieldStateInfo final
{
public:
	void init(std::shared_ptr<GameConfig> config);

	bool isDeltaEmpty() const { return deleted_position_.empty() && cells_state_.empty() && bacterium_state_.empty(); }

	void clearDelta()
	{
		deleted_position_.clear();
		cells_state_.clear();
		bacterium_state_.clear();
	}

	const std::vector<GrassInfo>& getCellInfo() const { return cells_state_; }

	const std::vector<BacteriumInfo>& getBacteriumInfo() const { return bacterium_state_; }

	const std::vector<DeletedPosition>& getDeletedPosition() const { return deleted_position_; }

	std::shared_ptr<GameConfig> getGameConfig() const {return config_;}

	const std::vector<int>& getSubscription() const { return game_subscription_; }

	void setSubscription(const std::vector<int>& subs) { game_subscription_ = subs; }

	void deleteSubscription(int id_channel) { game_subscription_.erase(std::remove(game_subscription_.begin(), game_subscription_.end(), id_channel), game_subscription_.end()); }

	void addSubscription(int id_channel) { game_subscription_.push_back(id_channel); }

	void update();

	void reset();

private:
	std::vector<DeletedPosition> deleted_position_;
	std::vector<GrassInfo> cells_state_;
	std::vector<BacteriumInfo> bacterium_state_;

	std::shared_ptr<FieldState> game_state_ = std::make_shared<FieldState>();
	std::shared_ptr<GameConfig> config_ = std::make_shared<GameConfig>();
	std::vector<int> game_subscription_;

	void fillVectorsInfo(/*const DeltaGameState& delta*/);
};

