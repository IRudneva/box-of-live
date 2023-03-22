#pragma once
#include <mutex>
#include <queue>


//#include "deserialize_packet.h"

template <typename PacketType>
class SharedPacketQueue
{
public:
	void pushPacket(PacketType packet)
	{
			std::lock_guard<std::mutex> lock(m);
			queue_.push(packet);
		//	std::cout << "Thread::" << std::this_thread::get_id() << " packet :" << (int)queue_.back()->packet.header.packet_type << " type, size " << sizeof(queue_.back()->packet) << " pushed." << std::endl;
	}

	bool hasPacket() const
	{
		if (queue_.empty())
			return false;

		return true;
	}

	PacketType popPacket()
	{
		std::lock_guard<std::mutex> lock(m);
		auto buff_packet = queue_.front();
		queue_.pop();
	//	std::cout << "Thread::" << std::this_thread::get_id() << " packet :" << (int)buff_packet->packet.header.packet_type << " type, size " << sizeof(buff_packet->packet) << " pop." << std::endl;
		return buff_packet;
	}

private:
	std::mutex m;
	std::queue<PacketType> queue_;
};