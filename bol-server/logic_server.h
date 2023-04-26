#pragma once
#include <atomic>
#include <thread>

#include "db_handler.h"
#include "log_duration.h"
#include "packet_reader.h"
#include "srv_manager.h"
#include "shared_packet_queue.h"

class LogicServer
{
public:
	explicit LogicServer(std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue)
	: queue_(queue)
	{
		SrvManager::getInstance()->initState(db_handler_->getRoomsData(), db_handler_->getFieldStateData(), db_handler_->getBacteriumData());
	}

	void runLogicLoop();

	void stopLogicLoop();

private:
	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue_;
	std::atomic_bool thread_queue_is_run_ = false;
	std::thread thread_queue_;

	std::shared_ptr<DbHandler> db_handler_ = std::make_shared<DbHandler>();
	std::atomic_bool thread_db_is_run_ = false;
	std::thread thread_db_;

	Timer timer_for_save_data_{ std::chrono::milliseconds(5000) };
	Timer timer_for_game_{ std::chrono::milliseconds(30) };

	void handleQueue();

	void handleDatabase();
};
