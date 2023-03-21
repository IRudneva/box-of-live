#include "cli_shared_packet_queue.h"
#include "deserialize_packet.h"
#include <iostream>

void SharedPacketQueue::pushPacket(std::shared_ptr<DeserializePacket> packet)
{
	std::lock_guard<std::mutex> lock(m);
	queue_.push(packet);
	std::cout << "Thread::" << std::this_thread::get_id() << " packet :" << (int)packet->type << " type, size " << (int)packet->type << " pushed." << std::endl;
}

bool SharedPacketQueue::hasPacket() const {
	if (queue_.empty())
		return false;

	return true;
}

std::shared_ptr<DeserializePacket> SharedPacketQueue::popPacket()
{
	std::lock_guard<std::mutex> lock(m);
	auto buff_packet = queue_.front();
	queue_.pop();
	std::cout << "Thread::" << std::this_thread::get_id() << " packet :" << (int)buff_packet->type << " type, size " << (int)buff_packet->type << " pop." << std::endl;
	return buff_packet;
}