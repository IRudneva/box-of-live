#include "pch_server.h"
#include "room_state.h"
#include <utility>
#include "data_for_save.h"
#include "db_payload.h"
#include "log_duration.h"

void RoomState::init(std::shared_ptr<GameConfig> config)
{
	config_ = std::move(config);
	game_state_->initConfig(config_);
}

void RoomState::update()
{
	if (!is_run_)
		return;

	auto delta = game_state_->getDeltaGameState();

	if (delta.empty())
	{
		game_state_->update();
	}
	//////////////////////////?????????????????????????????????????
	auto deleted_positions = getDeletedPosition(delta);
	auto grass_positions = getGrassInfo(delta);
	auto bacterium_info = getBacteriumInfo(delta);

	sendSubscription(deleted_positions, grass_positions, bacterium_info);

	DbSaveRoomState info = { id_room_, false };
	info.formDataCells(deleted_positions, grass_positions, bacterium_info);

	DbPayload::getInstance()->updateCellsRoomState(id_room_, info);
}

std::vector<DeletedPosition> RoomState::getDeletedPosition(const DeltaGameState& delta)
{
	std::vector<DeletedPosition> deleted_position;
	for (const auto& pos : delta.getDeletedPositions())
	{
		deleted_position.emplace_back(pos.x, pos.y);
	}
	return deleted_position;
}

std::vector<GrassInfo> RoomState::getGrassInfo(const DeltaGameState& delta)
{
	std::vector<GrassInfo> grass_state;
	for (const auto& pos : delta.getUpdatedPositions())
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

std::vector<BacteriumInfo> RoomState::getBacteriumInfo(const DeltaGameState& delta)
{
	std::vector<BacteriumInfo> bacterium_state;
	for (const auto& pos : delta.getUpdatedPositions())
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

void RoomState::sendSubscription(const std::vector<DeletedPosition>& del_inf, const std::vector<GrassInfo>& grass_inf, const std::vector<BacteriumInfo>& bact_inf)
{
	const server_packet::PTRoomState game_state(id_room_,
		grass_inf,
		bact_inf,
		del_inf);

	NetworkServer::getInstance()->sendPacketAllClients(game_state);
}

void RoomState::setColorByBacteriumMap(std::map<int, SrvColor> color_map)
{
	color_bacterium_by_type_ = color_map;
}

void RoomState::setColorByBacteriumId(int id)
{
	SrvColor color;
	color.red = getRandomInt(0, 255);
	color.blue = getRandomInt(0, 255);
	color.green = getRandomInt(0, 255);

	color_bacterium_by_type_.insert({ id,color });
}

