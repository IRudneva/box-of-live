#include "cli_manager.h"
#include "hv/TcpClient.h"
#include "hv/htime.h"
#include <memory>


class Client
{
public:
	void run()
	{
		if (initSocket(1234))
		{
			client_.onConnection = [&](const hv::SocketChannelPtr& channel) {

				std::string peeraddr = channel->peeraddr();
				if (channel->isConnected()) {
					channel_ = channel;
					printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				}
				else {
					printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				}
				if (client_.isReconnect()) {
					channel_ = channel;
					printf("reconnect cnt=%d, delay=%d\n", client_.reconn_setting->cur_retry_cnt, client_.reconn_setting->cur_delay);
				}
			};


			client_.onMessage = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
				//uint8_t* it = (uint8_t*)buf->data();
				//size_t sizeLeft = buf->size();
				//do {
				//	sizeLeft = reader.readData(&it, sizeLeft);
				//	if (reader.isAllDataComplete()) {
				//		// reader->getPacket(); // для дальнейшей обработки
				//		reader.reset();
				//	}
				//} while (sizeLeft > 0);


			/*	auto p = msgpack::unpack<Packet>((uint8_t*)buf->data(), buf->size());
				if (!p.room_list.empty()) {
					for (const auto&[id, room] : p.room_list)
					{
						std::cout << "room " << id << " state " << room.field_state << std::endl;;
					}
				}*/
			};


			client_.start();

			if(channel_ != nullptr && channel_->isConnected())
				ui_manager_->initGameUI(channel_);

			while (true) {

				if (client_.isConnected()) break;
				client_.startReconnect();
			}

		}
	}

	bool initSocket(int port)
	{
		int connfd = client_.createsocket(port);
		if (connfd < 0) {
			return false;
		}
		printf("client connect to port %d, connfd=%d ...\n", port, connfd);
		return true;
	}

private:
	hv::TcpClient client_;
	hv::SocketChannelPtr channel_ = nullptr;
	std::unique_ptr<ClientManager> ui_manager_ = std::make_unique<ClientManager>();

};

int main()
{
	Client client;
	client.run();
	return 0;
}
