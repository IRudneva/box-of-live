#pragma once
#include "bacterium.h"
#include "field_state.h"
#include "network_server.h"
#include "server_packet.h"

class RoomState final
{
public:
	RoomState() = default;

	explicit RoomState(int id, bool status) : is_run_(status), id_room_(id) { }

	void update() const;

	void reset() const { game_state_->restart(); }

	void sendSubscription(const std::vector<DeletedPosition>& del_inf, const std::vector<GrassInfo>& grass_inf, const std::vector<BacteriumInfo>& bact_inf) const;

	const std::vector<int>& getSubscription() const { return game_subscription_; }

	void initConfig(std::shared_ptr<GameConfig> config) const;

	void initBacteriumColors(int count);

	bool getStatus() const { return is_run_; }

	int getIdRoom() const { return id_room_; }

	void setActivateStatus(bool status)
	{
		is_run_ = status;

		const server_packet::PTActivateRoom pt_init_game(
			is_run_
		);
		for (const auto& client : game_subscription_)
		{
			NetworkServer::getInstance()->sendPacket(static_cast<uint32_t>(client), pt_init_game);
		}
	}

	std::shared_ptr<GameConfig> getGameConfig() const { return game_state_->getGameConfig(); }

	void deleteSubscription(int id_channel) { game_subscription_.erase(std::remove(game_subscription_.begin(), game_subscription_.end(), id_channel), game_subscription_.end()); }

	void addSubscription(int id_channel) { game_subscription_.push_back(id_channel); }

	const std::map<int, std::shared_ptr<Cell>>& getAllCellInfo() const { return game_state_->getData(); }

	void addGrass(int x, int y) const { game_state_->addGrass(x, y); }

	void addSuperGrass(int x, int y) { game_state_->addSuperGrass(x, y); }

	void addEffect(int x, int y) { game_state_->addEffectByBacterium(x, y); }

	void addBacterium(int x, int y, int id_type, int energy) const { game_state_->addBacterium(x, y, id_type, energy); }

	const std::map<int, SrvColor>& getColorsBacterium() const { return color_bacterium_by_type_; }

	void setColorByBacteriumMap(const std::map<int, SrvColor>& color_map);

private:
	std::shared_ptr<FieldState> game_state_ = std::make_shared<FieldState>();
	std::vector<int> game_subscription_;

	bool is_run_ = false;
	int id_room_ = 0;

	std::map<int, SrvColor> color_bacterium_by_type_;

	SrvColor createColorByBacteriumId(int id) const;
};

