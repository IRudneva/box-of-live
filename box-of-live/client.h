#pragma once
#include "gui_client_manager.h"
#include "hv/TcpClient.h"

class Client
{
public:
	void run()
	{
		if(initSocket(1234))
		{
			client_.onConnection = [&](const hv::SocketChannelPtr& channel) {
				std::string peeraddr = channel->peeraddr();
				if (channel->isConnected()) {
					gui_manager_->startLoop();
					printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				}
				else {
					printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				}
			};
			client_.onMessage = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
				printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
			};
			client_.start();
		}
	}

	bool initSocket(int port)
	{
		int connfd = client_.createsocket(port);
		if (connfd < 0) {
			return false;
		}
		return true;
	}

private:
	hv::TcpClient client_;
	std::shared_ptr<GuiClientManager> gui_manager_ = std::make_shared<GuiClientManager>();

};
