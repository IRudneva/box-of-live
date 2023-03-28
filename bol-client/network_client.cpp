#include "pch_client.h"
#include "network_client.h"

#include <iostream>

NetworkClient*  NetworkClient::p_instance = 0;
NetworkClientDestroyer  NetworkClient::destroyer;

NetworkClientDestroyer::~NetworkClientDestroyer() {
	delete p_instance;
}
void  NetworkClientDestroyer::initialize (NetworkClient* p) {
	p_instance = p;
}
NetworkClient&  NetworkClient::getInstance() {
	if (!p_instance) {
		p_instance = new  NetworkClient();
		destroyer.initialize(p_instance);
	}
	return *p_instance;
}

void NetworkClient::run()
{
	if (initSocket(1234))
	{
		client_.onConnection = [&](const BOLTcpClient::TSocketChannelPtr& channel) {

			std::string peeraddr = channel->peeraddr();
			if (channel->isConnected()) {
				printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				linkChannel(channel);
			}
			else {
				printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				unlinkChannel();
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
					auto type = channel->reader_.getPacketType();
					auto data = channel->reader_.getData();
					auto packet = ServerPacketBuilder::getPacket(type, data);
					std::cout << "thread:: " << std::this_thread::get_id() << std::endl;
					queue_->pushPacket(packet);
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

void NetworkClient::sendPacket(std::shared_ptr<ClientPacket> packet)
{
	if (!checkChannelIsValid())
		return;
	PacketWriter<std::shared_ptr<ClientPacket>> writer;
	std::vector<uint8_t> s_packet = writer.serialize(packet);
	channel_->write(s_packet.data(), (int)s_packet.size());
	std::cout << "send packet!" << std::endl;
}

void NetworkClient::linkChannel(const BOLTcpClient::TSocketChannelPtr& channel)
{
	std::lock_guard<std::mutex> lock(m_);
	channel_ = channel;
}

void NetworkClient::unlinkChannel()
{
	std::lock_guard<std::mutex> lock(m_);
	channel_ = nullptr;
}

bool NetworkClient::checkChannelIsValid()
{
	std::lock_guard<std::mutex> lock(m_);
	if (channel_ == nullptr)
		return false;
	return true;
}