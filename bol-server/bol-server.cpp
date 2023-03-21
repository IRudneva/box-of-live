#include "hv/TcpServer.h"
#include "srv_manager.h"

class Server
{
public:
	void run()
	{
		if (initSocket(1234))
		{
			printf("server listen on port %d ... \n", 1234);
			server_.onConnection = [&](const hv::SocketChannelPtr& channel) {

				std::string peeraddr = channel->peeraddr();
				if (channel->isConnected()) {
					printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				}
				else {
					printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				}
			};

			NetworkPacketReader reader;

			server_.onMessage = [&reader, this](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
				uint8_t* it = (uint8_t*)buf->data();
				size_t sizeLeft = buf->size();
				do {
					sizeLeft = reader.readNetworkPacket(&it, sizeLeft);
					if (reader.isAllDataComplete()) {
						auto packet = reader.getDeserializePacket(); // для дальнейшей обработки
						std::cout << (int)packet->type << " type packed received." << std::endl;
						srv_manager_->handlePacket(packet, channel);
					}
				} while (sizeLeft > 0);
			
			};


			server_.setThreadNum(4);
			server_.start();


			std::string str;
			while (std::getline(std::cin, str)) {
				if (str == "close") {
					server_.closesocket();
				}
				else if (str == "start") {
					server_.start();
				}
				else if (str == "stop") {
					server_.stop();
					break;
				}
				else {
					server_.broadcast(str.data(), str.size());
				}
			}
		}
	}

	bool initSocket(int port)
	{
		int listenfd = server_.createsocket(port);
		if (listenfd < 0) {
			return false;
		}
		return true;
	}

private:
	hv::TcpServer server_;
	std::unique_ptr<SrvManager> srv_manager_ = std::make_unique<SrvManager>();

};

int main()
{
	Server server;
	server.run();
	return 0;
}