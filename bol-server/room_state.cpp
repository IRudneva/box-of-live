#include "pch_server.h"
#include "room_state.h"
#include <utility>
#include "bacterium.h"
#include "request_handler.h"

void RoomState::init(std::shared_ptr<GameConfig> config)
{
	config_ = std::move(config);
	game_state_->initConfig(config_);
}

void RoomState::update()
{
	game_state_->update();
	auto delta = game_state_->getDeltaGameState();
	sendSubscription(delta);

	RequestHandler::getInstance()->updateRoomCells(id_room_, getDeletedPosition(delta), getGrassInfo(delta), getBacteriumInfo(delta));
}

void RoomState::reset()
{
	game_state_->restart();
}
