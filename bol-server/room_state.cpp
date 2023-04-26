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

	auto deleted_positions = getDeletedPosition(delta);
	auto grass_positions = getGrassInfo(delta);
	auto bacterium_info = getBacteriumInfo(delta);

	sendSubscription(deleted_positions, grass_positions, bacterium_info);

	DbSaveRoomState info = { id_room_, false };
	info.formDataCells(deleted_positions, grass_positions, bacterium_info);

	DbPayload::getInstance()->updateCellsRoomState(id_room_, info);
}

void RoomState::reset()
{
	game_state_->restart();
}

