#pragma once
#include <iostream>

#include "packet_reader.h"
#include "srv_manager.h"
#include "shared_packet_queue.h"

class LogicServer
{
public:
	LogicServer()
	{
		std::lock_guard<std::mutex> lock(m);
		std::cout << "thread:: " << std::this_thread::get_id() << std::endl;
	}
	void run(std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue);

private:
	std::mutex m;
	std::unique_ptr<SrvManager> srv_manager_ = std::make_unique<SrvManager>();

};
