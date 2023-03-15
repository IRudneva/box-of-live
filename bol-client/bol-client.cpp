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
					printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
					// send(time) every 3s
					/*hv::setInterval(3000, [channel](hv::TimerID timerID) {
						if (channel->isConnected()) {
							if (channel->isWriteComplete()) {
								char str[DATETIME_FMT_BUFLEN] = { 0 };
								datetime_t dt = datetime_now();
								datetime_fmt(&dt, str);
								channel->write(str);
							}
						}
						else
						{
							hv::killTimer(timerID);
						}

					});*/
					//	gui_manager_->startLoop();
				}
				else {
					printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				}
				if (client_.isReconnect()) {
					printf("reconnect cnt=%d, delay=%d\n", client_.reconn_setting->cur_retry_cnt, client_.reconn_setting->cur_delay);
				}
			};
			client_.onMessage = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
				auto p = msgpack::unpack<Packet>((uint8_t*)buf->data(), buf->size());
				if (!p.room_list.empty()) {
					for (const auto&[id, room] : p.room_list)
					{
						std::cout << "room " << id << " state " << room.field_state << std::endl;;
					}
				}
			};

			client_.start();

			std::string str;
			while (std::getline(std::cin, str)) {
				if (str == "close") {
					client_.closesocket();
				}
				else if (str == "start") {
					client_.start();
				}
				else if (str == "stop") {
					client_.stop();
					break;
				}
				else if (str == "get room list")
				{
					Msg m;
					auto data = msgpack::pack(m);
					client_.send(data.data(), data.size());
				}
				else {
					if (!client_.isConnected()) break;
					client_.send(str);
				}
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
	std::shared_ptr<ClientManager> gui_manager_ = std::make_shared<ClientManager>();

};

int main()
{
	Client client;
	client.run();
	return 0;
}
