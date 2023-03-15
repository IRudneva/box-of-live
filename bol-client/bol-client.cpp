#include <iostream>
//#include "cli_manager.h"
#include <TGUI/TGUI.hpp>
#include "hv/TcpClient.h"
#include "hv/htime.h"
#include <memory>
#include "packet_reader.h"
#include "msgpack.hpp"

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

			PacketReader reader;

			client_.onMessage = [&reader](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
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
					Packet pt_get_rooms = { {PacketType::PT_MSG_GET_ROOM_LIST, 0} };
					client_.send(&pt_get_rooms, sizeof(PacketHeader));
					std::vector<int> a(200, 1);

					/*Packet pt_add_new_room = { {PacketType::PT_ROOM_LIST, 3} };
					for (auto i = a.begin(); i != a.end(); i++)
					{
						pt_add_new_room.data.push_back(msgpack::pack((*i)));
					}
					pt_add_new_room.data.push_back()
					client_.send(&pt_add_new_room, pt_add_new_room.header.data_size);*/
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
	//std::shared_ptr<ClientManager> gui_manager_ = std::make_shared<ClientManager>();

};

int main()
{
	Client client;
	client.run();
	return 0;
}
