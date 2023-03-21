#pragma once
#include <iostream>
#include <mutex>
#include <queue>
#include "packet_reader.h"

inline std::mutex SHARED_QUEUE_MUTEX;

class SharedPacketQueue
{
public:
	void pushPacket(std::shared_ptr<DeserializePacket> packet)
	{
		std::lock_guard<std::mutex> lock(SHARED_QUEUE_MUTEX);
		pushed_packet_ = packet;
		std::cerr << "Thread::" << std::this_thread::get_id() << " packet :" << (int)packet->type << " type, size " << (int)packet->type << " pushed." << std::endl;
	}

	bool hasPacket() const {
		if (queue_.empty())
			return false;

		return true;
	}

	void handlePushedPacket() {
		if (pushed_packet_ == nullptr)
			return;

		queue_.push(pushed_packet_);
		pushed_packet_ = nullptr;
	}

	std::shared_ptr<DeserializePacket> popPacket()
	{
		std::lock_guard<std::mutex> lock(SHARED_QUEUE_MUTEX);
		auto buff_packet = queue_.front();
		queue_.pop();
		std::cerr <<"Thread::" <<std::this_thread::get_id() << " packet :" << (int)buff_packet->type << " type, size " << (int)buff_packet->type << " pop." << std::endl;
		return buff_packet;
	}


private:
	std::queue<std::shared_ptr<DeserializePacket>> queue_;
	std::shared_ptr<DeserializePacket> pushed_packet_;
};