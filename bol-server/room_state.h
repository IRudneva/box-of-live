#pragma once
#include "bacterium.h"
#include "field_state.h"
#include "network_server.h"
#include "server_packet.h"

class RoomState final
{
public:
	RoomState()
	{
		srand(static_cast<unsigned int>(time(NULL)));
		for (int i = 1; i < NUMBER_BACTERIAL_COLONIES; ++i)
		{
			setColorByBacteriumId(i);
		}
	}

	explicit RoomState(int id, bool status) : is_run_(status), id_room_(id)
	{
		srand(static_cast<unsigned int>(time(NULL)));
		for (int i = 1; i < NUMBER_BACTERIAL_COLONIES; ++i)
		{
			setColorByBacteriumId(i);
		}
	}

	void init(std::shared_ptr<GameConfig> config);

	bool getStatus() const { return is_run_; }

	void activate(bool status) { is_run_ = status; }

	std::shared_ptr<GameConfig> getGameConfig() const {return config_;}

	const std::vector<int>& getAllSubscription() const { return game_subscription_; }

	void deleteSubscription(int id_channel) { game_subscription_.erase(std::remove(game_subscription_.begin(), game_subscription_.end(), id_channel), game_subscription_.end()); }

	void addSubscription(int id_channel) { game_subscription_.push_back(id_channel); }

	const std::map<int, std::shared_ptr<Cell>>& getAllCellInfo() const { return game_state_->getData(); }

	std::vector<BacteriumInfo> getAllBacteriumInfo() const
	{
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

	void reset() { game_state_->restart(); };;

	void addGrass(int x, int y) { game_state_->addGrass(x, y); }

	void addBacterium(int x, int y, int id_type, int energy) { game_state_->addBacterium(x, y, id_type, energy); }

	std::vector<DeletedPosition> getDeletedPosition(const DeltaGameState& delta);

	std::vector<GrassInfo> getGrassInfo(const DeltaGameState& delta);

	std::vector<BacteriumInfo> getBacteriumInfo(const DeltaGameState& delta);

	const std::map<int, SrvColor>& getColorByBacterium() { return color_bacterium_by_type_; }

	void sendSubscription(const std::vector<DeletedPosition>& del_inf, const std::vector<GrassInfo>& grass_inf, const std::vector<BacteriumInfo>& bact_inf);

	void setColorByBacteriumMap(std::map<int, SrvColor> color_map);

private:
	std::shared_ptr<FieldState> game_state_ = std::make_shared<FieldState>();
	std::shared_ptr<GameConfig> config_ = std::make_shared<GameConfig>();
	std::vector<int> game_subscription_;

	bool is_run_ = false;
	int id_room_ = 0;

	std::map<int, SrvColor> color_bacterium_by_type_;

	void setColorByBacteriumId(int id);
};

