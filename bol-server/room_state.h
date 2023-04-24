#pragma once
#include "bacterium.h"
#include "field_state.h"
#include "network_server.h"
#include "server_packet.h"

class RoomState final
{
public:
	RoomState() = default;

	explicit RoomState(int id, bool status) : id_room_(id), is_run_(status) {}

	void init(std::shared_ptr<GameConfig> config);

	void activate(bool status)
	{
		is_run_ = status;
	}

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

	void reset();

	void addGrass(int x, int y)
	{
		game_state_->addGrass(x, y);
	}

	void addBacterium(int x, int y, int id_type, int energy)
	{
		game_state_->addBacterium(x, y, id_type, energy);
	}

	std::vector<DeletedPosition> getDeletedPosition(const DeltaGameState& delta)
	{
		std::vector<DeletedPosition> deleted_position;
		for (const auto& pos : delta.deleted_cells)
		{
			deleted_position.emplace_back(pos.x, pos.y);
		}
		return deleted_position;
	}

	std::vector<GrassInfo> getGrassInfo(const DeltaGameState& delta)
	{
		std::vector<GrassInfo> grass_state;
		for (const auto& pos : delta.update_cells)
		{
			if (auto cell = game_state_->getCellInPosition(pos); cell != nullptr)
			{
				if (cell->getCellType() == TypeCell::GRASS)
				{
					GrassInfo inf_grass(pos.x, pos.y);
					grass_state.emplace_back(inf_grass);
				}
			}
		}
		return grass_state;
	}

	std::vector<BacteriumInfo> getBacteriumInfo(const DeltaGameState& delta)
	{
		std::vector<BacteriumInfo> bacterium_state;
		for (const auto& pos : delta.update_cells)
		{
			if (auto cell = game_state_->getCellInPosition(pos); cell != nullptr)
			{
				if (cell->getCellType() == TypeCell::BACTERIUM)
				{
					Cell& a = *cell;
					auto bacterium = dynamic_cast<Bacterium&>(a);
					BacteriumInfo inf_bac(pos.x, pos.y, bacterium.getIdType(), bacterium.getEnergy());
					bacterium_state.emplace_back(inf_bac);
				}
			}
		}
		return bacterium_state;
	}

	void sendSubscription(const DeltaGameState& delta)
	{
		const auto deleted_position = getDeletedPosition(delta);
		const auto grass_state = getGrassInfo(delta);
		const auto bacterium_state = getBacteriumInfo(delta);

	/*	for (const auto& pos : delta.update_cells)
		{
			if (auto cell = game_state_->getCellInPosition(pos); cell != nullptr)
			{
				if (cell->getCellType() == TypeCell::BACTERIUM)
				{
					Cell& a = *cell;
					auto bacterium = dynamic_cast<Bacterium&>(a);
					BacteriumInfo inf_bac(pos.x, pos.y, bacterium.getIdType(), bacterium.getEnergy());
					bacterium_state.emplace_back(inf_bac);
				}
				else if (cell->getCellType() == TypeCell::GRASS)
				{
					GrassInfo inf_grass(pos.x, pos.y);
					grass_state.emplace_back(inf_grass);
				}
			}
		}*/

		const server_packet::PTRoomState game_state(id_room_,
			grass_state,
			bacterium_state,
			deleted_position);

		for (const auto& client : getAllSubscription())
		{
			NetworkServer::getInstance()->sendPacket(static_cast<uint32_t>(client), game_state);
		}
	}

private:
	std::shared_ptr<FieldState> game_state_ = std::make_shared<FieldState>();
	std::shared_ptr<GameConfig> config_ = std::make_shared<GameConfig>();
	std::vector<int> game_subscription_;

	bool is_run_ = false;
	int id_room_ = 0;


};

