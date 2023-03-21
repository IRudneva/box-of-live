#pragma once
#include <memory>
#include <mutex>
#include <queue>

struct DeserializePacket;

class SharedPacketQueue
{
public:
	void pushPacket(std::shared_ptr<DeserializePacket> packet);

	bool hasPacket() const;

	std::shared_ptr<DeserializePacket> popPacket();

private:
	std::mutex m;
	std::queue<std::shared_ptr<DeserializePacket>> queue_;
};