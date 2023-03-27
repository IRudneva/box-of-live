#pragma once
#include <iostream>
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
	//std::mutex m_;
	std::unique_ptr<SrvManager> srv_manager_ = std::make_unique<SrvManager>();
	std::shared_ptr<SharedPacketQueue<PacketWithIdChannel>> queue_;
	std::atomic_bool is_run_ = false;
	std::thread thread_;
	//BOLThread<void> thread_;

	void handleQueue() const
	{
		while (is_run_)
		{
			if (queue_->hasPacket())
			{
				srv_manager_->handlePacket(queue_->popPacket());
			}
		}
	}

};
