#pragma once
#include "packet_reader.h"
#include "srv_manager.h"
#include "srv_shared_packet_queue.h"

class LogicServer
{
public:
	void run(std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue);

private:
	std::unique_ptr<SrvManager> srv_manager_ = std::make_unique<SrvManager>();

};
