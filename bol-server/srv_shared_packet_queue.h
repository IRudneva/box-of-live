#pragma once
#include <mutex>
#include <queue>

template <typename PacketType>
class SharedPacketQueue
{
public:
	void pushPacket(PacketType packet)
	{
			std::lock_guard<std::mutex> lock(m);
			queue_.push(packet);
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
		return buff_packet;
	}

private:
	std::mutex m;
	std::queue<PacketType> queue_;
};