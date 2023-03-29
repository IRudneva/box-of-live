#include "pch_server.h"
#include "network_server.h"
#include <iostream>

NetworkServer* NetworkServer::p_instance = 0;
NetworkServerDestroyer NetworkServer::destroyer;

NetworkServerDestroyer::~NetworkServerDestroyer() {
	delete p_instance;
}
void NetworkServerDestroyer::initialize(NetworkServer* p) {
	p_instance = p;
}
NetworkServer& NetworkServer::getInstance() {
	if (!p_instance) {
		p_instance = new NetworkServer();
		destroyer.initialize(p_instance);
	}
	return *p_instance;
}

void NetworkServer::init()
{
	std::cout << "NS::run thread " << std::this_thread::get_id() << std::endl;
	if (initSocket(1234))
	{
		printf("server listen on port %d ... \n", 1234);
		server_.onConnection = [this](const BOLTcpServer::TSocketChannelPtr& channel) {

			std::string peeraddr = channel->peeraddr();
			if (channel->isConnected()) {
				printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				addChannel(channel);
			}
			else {
				printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
				//deleteChannel(channel);
			}
		};

		server_.onMessage = [this](const BOLTcpServer::TSocketChannelPtr& channel, hv::Buffer* buf) {
			uint8_t* it = (uint8_t*)buf->data();
			size_t size_left = buf->size();
			do {
				size_left = channel->reader_.readNetworkPacket(&it, size_left);
				if (channel->reader_.isAllDataComplete()) {
					auto type = channel->reader_.getPacketType();
					auto data = channel->reader_.getData();
					auto packet = ClientPacketBuilder::getPacket(type, data);
					client_packet::PacketWithIdChannel packet_with_id{ packet, channel->id() };
					queue_->pushPacket(packet_with_id);
				}
			} while (size_left > 0);

		};

		server_.setThreadNum(4);
		server_.start();
	}
}

void NetworkServer::sendPacket(uint32_t id_channel, const server_packet::ServerPacket& packet)
{
	//проверить, подключен ли клиент
	if (findChannel(id_channel))
	{
		if (auto client = channel_map_[id_channel].lock())
		{
			PacketWriter writer;
			auto s_packet = writer.serialize(packet);
			client->write(s_packet.data(), (int)s_packet.size());
		}
	}
}

void NetworkServer::addChannel(const BOLTcpServer::TSocketChannelPtr& channel)
{
	std::lock_guard<std::mutex> lock(m_);
	std::weak_ptr<BOLSocketChannel> wp(channel);
	channel_map_[channel->id()] = wp;
}

void NetworkServer::deleteChannel(const BOLTcpServer::TSocketChannelPtr& channel)
{
	std::lock_guard<std::mutex> lock(m_);
	channel_map_.erase(channel->id());
}

bool NetworkServer::findChannel(uint32_t id_channel)
{
	std::lock_guard<std::mutex> lock(m_);
	auto client_it = channel_map_.find(id_channel);
	if (client_it != channel_map_.end())
		return true;
	return false;
}

bool NetworkServer::initSocket(int port)
{
	int listenfd = server_.createsocket(port);
	if (listenfd < 0) {
		return false;
	}
	return true;
}

void NetworkServer::start()
{
	server_.start();
}

void NetworkServer::stop()
{
	server_.stop();
}