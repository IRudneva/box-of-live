#include "pch_client.h"
#include "network_client.h"

#include <iostream>

void NetworkClient::run()
{
	if (initSocket(1234))
	{
		client_.onConnection = [&](const BOLTcpClient::TSocketChannelPtr& channel) {

			std::string peeraddr = channel->peeraddr();
			if (channel->isConnected()) {
				printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				channel_ = channel;
			}
			else {
				printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				channel_ = nullptr;
			}
			if (client_.isReconnect()) {
				printf("reconnect cnt=%d, delay=%d\n", client_.reconn_setting->cur_retry_cnt, client_.reconn_setting->cur_delay);
			}
		};

		client_.onMessage = [this](const BOLTcpClient::TSocketChannelPtr& channel, hv::Buffer* buf) {
			uint8_t* it = (uint8_t*)buf->data();
			size_t size_left = buf->size();
			do {
				size_left = channel->reader_.readNetworkPacket(&it, size_left);
				if (channel->reader_.isAllDataComplete()) {
					auto packet = pack(channel);
					std::cout << "thread:: " << std::this_thread::get_id() << std::endl;
					queue_->pushPacket(*packet);
				}
			} while (size_left > 0);

		};

		client_.start();
	}
}

bool NetworkClient::initSocket(int port)
{
	int connfd = client_.createsocket(port);
	if (connfd < 0) {
		return false;
	}
	printf("client connect to port %d, connfd=%d ...\n", port, connfd);
	return true;
}

std::shared_ptr<DeserializePacketWithIdChannel> NetworkClient::pack(const BOLTcpClient::TSocketChannelPtr& channel) const
{
	auto packet = channel->reader_.getDeserializePacket(); // для дальнейшей обработки
	packet->id_channel = channel->id();
	std::cout << (int)packet->packet->type << " type packed received." << "channel id " << (int)packet->id_channel << std::endl;
	return packet;
}
