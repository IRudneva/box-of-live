#pragma once
#include "cli_manager.h"
#include "hv/TcpClient.h"

//#include "cli_manager.h"
//#include "hv/TcpClient.h"
//#include "hv/htime.h"
//#include <memory>

class Client
{
public:
	void run();

	bool initSocket(int port);

private:
	hv::TcpClient client_;
	hv::SocketChannelPtr channel_ = nullptr;
	std::unique_ptr<ClientManager> ui_manager_ = std::make_unique<ClientManager>();
};