#pragma once
#include "bacterium.h"
#include "field_state.h"

class FieldStateInfo final
{
public:
	void init(std::shared_ptr<GameConfig> config);

	bool isDeltaEmpty() const { return deleted_position_.empty() && grass_state_.empty() && bacterium_state_.empty(); }

	void clearDelta()
	{
		deleted_position_.clear();
		grass_state_.clear();
		bacterium_state_.clear();
	}

	const std::vector<GrassInfo>& getGrassInfo() const { return grass_state_; }

	const std::vector<BacteriumInfo>& getBacteriumInfo() const { return bacterium_state_; }

	const std::vector<DeletedPosition>& getDeletedPosition() const { return deleted_position_; }

	std::shared_ptr<GameConfig> getGameConfig() const {return config_;}

	const std::vector<int>& getSubscription() const { return game_subscription_; }

	void deleteSubscription(int id_channel) { game_subscription_.erase(std::remove(game_subscription_.begin(), game_subscription_.end(), id_channel), game_subscription_.end()); }

	void addSubscription(int id_channel) { game_subscription_.push_back(id_channel); }

	void fillAllCellsInGameState();

	std::map<int, std::shared_ptr<Cell>> getAllCellInfo() const { return game_state_->getData(); }

	std::vector<BacteriumInfo> getAllBacteriumInfo() {
		std::vector<BacteriumInfo> data;
		for (const auto&[id, cell] : game_state_->getData())
		{
			const auto pos = cell->getPosition();
			if (cell->getCellType() == TypeCell::BACTERIUM)
			{
				Cell& a = *cell;
				auto bacterium = dynamic_cast<Bacterium&>(a);
				BacteriumInfo inf_bac(pos.x, pos.y, bacterium.getIdType(), bacterium.getEnergy());
				data.emplace_back(inf_bac);
			}
		}
		return data;
	}

	void update();

	void reset();

	void setState(int x, int y, TypeCell type)
	{
		switch (type)
		{
		case TypeCell::GRASS:
		{
			game_state_->addGrass(x, y);
		}
		case TypeCell::BACTERIUM:
		{
			/*	auto bac = std::make_shared<Bacterium>()
			game_state_->addBacterium();*/
		}
		}
	}
	void addGrass(int x, int y)
	{
		game_state_->addGrass(x, y);
	}

	void addBacterium(int x, int y, int id_type, int energy)
	{
		game_state_->addBacterium(x, y, id_type, energy);
	}
					

private:
	std::vector<DeletedPosition> deleted_position_;
	std::vector<GrassInfo> grass_state_;
	std::vector<BacteriumInfo> bacterium_state_;

	std::shared_ptr<FieldState> game_state_ = std::make_shared<FieldState>();
	std::shared_ptr<GameConfig> config_ = std::make_shared<GameConfig>();
	std::vector<int> game_subscription_;

	void fillVectorsInfo(const DeltaGameState& delta);
};

