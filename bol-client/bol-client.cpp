#include "bol-client.h"
#include "packet_reader.h"

void Client::run()
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

		NetworkPacketReader reader;

		client_.onMessage = [&reader, this](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
			uint8_t* it = (uint8_t*)buf->data();
			size_t sizeLeft = buf->size();
			do {
				sizeLeft = reader.readNetworkPacket(&it, sizeLeft);

				if (reader.isAllDataComplete()) {
					PacketType type_packet = reader.getNetworkPacketHeader().value().packet_type;
					auto packet = reader.getDeserializePacket(); // для дальнейшей обработки

					std::cout << (int)packet->type << " type packed received." << std::endl;
					ui_manager_->handlePacket(packet, channel);
				}
			} while (sizeLeft > 0);

		};

		client_.start();

		if (channel_ != nullptr && channel_->isConnected())
			ui_manager_->initGameUI(channel_);

		while (true) {
			if (client_.isConnected()) break;
			client_.startReconnect();
		}

	}
}

bool Client::initSocket(int port)
{
	int connfd = client_.createsocket(port);
	if (connfd < 0) {
		return false;
	}
	printf("client connect to port %d, connfd=%d ...\n", port, connfd);
	return true;
}

