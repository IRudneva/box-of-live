#include "pch_server.h"
#include "network_server.h"
#include <iostream>

void NetworkServer::run()
{
	if (initSocket(1234))
	{
		printf("server listen on port %d ... \n", 1234);
		server_.onConnection = [this](const BOLTcpServer::TSocketChannelPtr& channel) {

			std::string peeraddr = channel->peeraddr();
			if (channel->isConnected()) {
				printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());


				////
				channel_map_[channel->id()] = channel->getContextPtr<BOLTcpServer::TSocketChannelPtr>();
			}
			else {
				printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
			}
		};

		server_.onMessage = [this](const BOLTcpServer::TSocketChannelPtr& channel, hv::Buffer* buf) {
			uint8_t* it = (uint8_t*)buf->data();
			size_t size_left = buf->size();
			do {
				size_left = channel->reader_.readNetworkPacket(&it, size_left);
				if (channel->reader_.isAllDataComplete()) {
					auto packet = pack(channel);
					//auto packet = channel->reader_.getDeserializePacket(); // для дальнейшей обработки
					//packet->id_channel = channel->id();
					//std::cout << (int)packet->packet.type << " type packed received." << std::endl;
					queue_->pushPacket(packet);
					//srv_manager_->handlePacket(packet, channel);
				}
			} while (size_left > 0);

		};

		server_.setThreadNum(4);
		server_.start();
	}
}


bool NetworkServer::initSocket(int port)
{
	int listenfd = server_.createsocket(port);
	if (listenfd < 0) {
		return false;
	}
	return true;
}

DeserializePacketWithIdChannel NetworkServer::pack(const BOLTcpServer::TSocketChannelPtr& channel) const
{
	auto packet = channel->reader_.getDeserializePacket(); // для дальнейшей обработки
	packet.id_channel = channel->id();
	std::cout << (int)packet.packet->type << " type packed received." << "channel id "<< (int)packet.id_channel << std::endl;
	return packet;
}
