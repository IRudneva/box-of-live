#pragma once
#include <atomic>
#include <thread>

#include "packet_reader.h"
#include "cli_manager.h"
#include "shared_packet_queue.h"

class LogicClient
{
public:
	explicit LogicClient(std::shared_ptr<SharedPacketQueue<std::shared_ptr<ServerPacket>>> queue) : queue_(queue) {}

	void runLoop();

	void stopLoop();

private:
	std::atomic_bool ui_is_run_ = false;
	std::atomic_bool logic_is_run_ = false;
	std::thread ui_thread_;
	std::thread logic_thread_;
	std::unique_ptr<ClientManager> ui_manager_ = std::make_unique<ClientManager>();
	std::shared_ptr<SharedPacketQueue<std::shared_ptr<ServerPacket>>> queue_;

	void handleQueue() const;

	void updateUI();
};