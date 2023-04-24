#pragma once
#include <atomic>
#include <thread>

#include "bol_database.h"
#include "packet_reader.h"
#include "srv_manager.h"
#include "shared_packet_queue.h"

class LogicServer
{
public:
	explicit LogicServer(std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue, std::shared_ptr<DatabaseHandler> db_handler)
	: queue_(queue),
	db_handler_(db_handler)
	{
		srv_manager_->initState(db_handler_->getRoomsData(), db_handler_->getFieldStateData(), db_handler_->getBacteriumData());
	}

	void runLogicLoop();

	void stopLogicLoop();

private:
	std::unique_ptr<SrvManager> srv_manager_ = std::make_unique<SrvManager>();
	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue_;
	std::atomic_bool thread_queue_is_run_ = false;
	std::thread thread_queue_;

	std::shared_ptr<DatabaseHandler> db_handler_;
	std::atomic_bool thread_db_is_run_ = false;
	std::thread thread_db_;

	Timer timer_for_save_data_{ std::chrono::milliseconds(60000) };

	void handleQueue() const;

	void handleDatabase();
};
