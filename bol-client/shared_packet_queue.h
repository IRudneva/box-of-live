#pragma once
#include <iostream>
#include <mutex>
#include <queue>
#include "packet_reader.h"

inline std::mutex SHARED_QUEUE_MUTEX;

class SharedPacketQueue
{
public:
	void pushPacket(std::shared_ptr<Packet> packet)
	{
		std::lock_guard<std::mutex> lock(SHARED_QUEUE_MUTEX);
		pushed_packet_ = packet;
		std::cerr << "Thread::" << std::this_thread::get_id() << "packet :" << (int)packet->header.packet_type << " type, size " << packet->header.data_size << " pushed." << std::endl;
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

	std::shared_ptr<Packet> popPacket()
	{
		std::lock_guard<std::mutex> lock(SHARED_QUEUE_MUTEX);
		auto buff_packet = queue_.front();
		queue_.pop();
		std::cerr <<"Thread::" <<std::this_thread::get_id() << "packet :" << (int)buff_packet->header.packet_type << " type, size " << buff_packet->header.data_size << " pop." << std::endl;
		return buff_packet;
	}


private:
	std::queue<std::shared_ptr<Packet>> queue_;
	std::shared_ptr<Packet> pushed_packet_;
};