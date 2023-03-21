#pragma once
#include "packet_reader.h"
#include "srv_manager.h"
#include <hv/TcpServer.h>

class Server
{
public:
	void run();

	bool initSocket(int port);

private:
	hv::TcpServer server_;
	std::unique_ptr<SrvManager> srv_manager_ = std::make_unique<SrvManager>();

};
