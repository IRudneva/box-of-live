#pragma once
#include "packet_reader.h"
#include <mutex>
#include <queue>
#include <memory>
#include <iostream>

inline std::mutex SHARED_QUEUE_MUTEX;

class SharedPacketQueue
{
public:
	void pushPacket(std::shared_ptr<DeserializePacket> packet);

	bool hasPacket() const;

	void handlePushedPacket();

	std::shared_ptr<DeserializePacket> popPacket();

private:
	std::queue<std::shared_ptr<DeserializePacket>> queue_;
	std::shared_ptr<DeserializePacket> pushed_packet_;
};