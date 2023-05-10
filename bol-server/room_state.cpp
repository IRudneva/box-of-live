#include "pch_server.h"
#include "room_state.h"
#include <utility>
#include "data_for_save.h"
#include "db_payload.h"
#include "grass.h"
#include "log_duration.h"

void RoomState::initConfig(std::shared_ptr<GameConfig> config) const { game_state_->initConfig(std::move(config)); }

void RoomState::initBacteriumColors()
{
	for (int i = 1; i < NUMBER_BACTERIAL_COLONIES; ++i)
	{
		color_bacterium_by_type_.insert({ i, createColorByBacteriumId(i) });
	}
}

SrvColor RoomState::createColorByBacteriumId(int id) const
{
	SrvColor color;
	color.red = getRandomInt(0, 255);
	color.blue = getRandomInt(0, 255);
	color.green = getRandomInt(0, 255);
	return color;
}

void RoomState::setColorByBacteriumMap(const std::map<int, SrvColor>& color_map)
{
	color_bacterium_by_type_ = std::move(color_map);
}

void RoomState::update() const
{
	if (!is_run_)
		return;

	auto delta = game_state_->delta_state_;

	if (delta.empty())
	{
		game_state_->update();
	}

	std::vector<DeletedPosition> deleted_positions;
	std::vector<GrassInfo> grass_positions;
	std::vector<BacteriumInfo> bacterium_info;

	for (const auto& pos : game_state_->delta_state_.getDeletedPositions())
	{
		deleted_positions.emplace_back(pos.x, pos.y);
	}
	for (const auto& pos : game_state_->delta_state_.getUpdatedPositions())
	{
		if (auto cell = game_state_->getCellInPosition(pos); cell != nullptr)
		{
			auto type = cell->getCellType();
			if (type == TypeCell::GRASS)
			{
				Cell& a = *cell;
				auto grass = std::move(dynamic_cast<Grass&>(a));
				GrassInfo inf_grass(pos.x, pos.y, grass.isSuperGrass());
				grass_positions.emplace_back(inf_grass);
				continue;
			}
			if (type == TypeCell::BACTERIUM)
			{
				Cell& a = *cell;
				auto bacterium = std::move(dynamic_cast<Bacterium&>(a));
				BacteriumInfo inf_bac(pos.x, pos.y, bacterium.getIdType(), bacterium.getEnergy());
				bacterium_info.emplace_back(inf_bac);
				continue;
			}
		}
	}

	sendSubscription(deleted_positions, grass_positions, bacterium_info);

	DbSaveRoomState info = { id_room_, false };
	info.formDataCells(deleted_positions, grass_positions, bacterium_info);

	DbPayload::getInstance()->updateCellsRoomState(info);

	game_state_->delta_state_.clear();

}


void RoomState::sendSubscription(const std::vector<DeletedPosition>& del_inf, const std::vector<GrassInfo>& grass_inf, const std::vector<BacteriumInfo>& bact_inf) const
{
	const server_packet::PTRoomState game_state(id_room_,
		grass_inf,
		bact_inf,
		del_inf);
	for(const auto& client : game_subscription_)
	{
		NetworkServer::getInstance()->sendPacket(static_cast<uint32_t>(client), game_state);
	}
}
