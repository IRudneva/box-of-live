#include "srv_shared_packet_queue.h"

void SharedPacketQueue::pushPacket(std::shared_ptr<DeserializePacket> packet)
{
	std::lock_guard<std::mutex> lock(SHARED_QUEUE_MUTEX);
	pushed_packet_ = packet;
	std::cout << "Thread::" << std::this_thread::get_id() << " packet :" << (int)packet->type << " type, size " << sizeof(packet) << " pushed." << std::endl;
}

bool SharedPacketQueue::hasPacket() const {
	if (queue_.empty())
		return false;

	return true;
}

void SharedPacketQueue::handlePushedPacket() {
	if (pushed_packet_ == nullptr)
		return;

	queue_.push(pushed_packet_);
	pushed_packet_ = nullptr;
}

std::shared_ptr<DeserializePacket> SharedPacketQueue::popPacket()
{
	std::lock_guard<std::mutex> lock(SHARED_QUEUE_MUTEX);
	auto buff_packet = queue_.front();
	queue_.pop();
	std::cout << "Thread::" << std::this_thread::get_id() << " packet :" << (int)buff_packet->type << " type, size " << sizeof(buff_packet) << " pop." << std::endl;
	return buff_packet;
}
