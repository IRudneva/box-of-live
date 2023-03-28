#pragma once
#include <atomic>
#include <thread>
#include "packet_reader.h"
#include "srv_manager.h"
#include "shared_packet_queue.h"

class LogicServer
{
public:
	explicit LogicServer(std::shared_ptr<SharedPacketQueue<PacketWithIdChannel>> queue) : queue_(queue){}

	void runLogicLoop();

	void stopLogicLoop();

private:
	std::unique_ptr<SrvManager> srv_manager_ = std::make_unique<SrvManager>();
	std::shared_ptr<SharedPacketQueue<PacketWithIdChannel>> queue_;
	std::atomic_bool is_run_ = false;
	std::thread thread_;

	void handleQueue() const;
};
