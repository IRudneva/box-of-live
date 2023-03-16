#include "srv_manager.h"
#include "hv/TcpServer.h"
#include <memory>
#include <iostream>
#include "packet_reader.h"
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

			PacketReader reader;

			server_.onMessage = [&reader, this](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
				uint8_t* it = (uint8_t*)buf->data();
				size_t sizeLeft = buf->size();
				do {
					sizeLeft = reader.readData(&it, sizeLeft);
					//if(sizeLeft == 0)
					//{
					//	auto pac = reader.getPacket(); // для дальнейшей обработки
					//	std::cout << (int)pac.header.packet_type << " type packed received." << std::endl;
					//}
					if (reader.isAllDataComplete()) {
						 auto pac = reader.getPacket(); // для дальнейшей обработки
						 std::cout << (int)pac.header.packet_type << " type packed received." << std::endl;
						reader.reset();
					}
				} while (sizeLeft > 0);
				/*Msg m = msgpack::unpack<Msg>((uint8_t*)buf->data(), buf->size());
				srv_manager_->handleMsg(m);
				Packet p = { PacketType::PT_ROOM_LIST, srv_manager_->getRoomList() };
				auto data = msgpack::pack(p);
				channel->write(data.data(), data.size());*/

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

			//gui_manager_->startLoop(); // это должно происходить после подключения - сейчас нужно, чтобы смотреть результат
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
//	std::shared_ptr<SrvManager> srv_manager_ = std::make_shared<SrvManager>();

};

int main()
{
	Server server;
	server.run();
	return 0;
}