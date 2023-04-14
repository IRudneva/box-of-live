#include "pch_server.h"
#include "network_server.h"

#include "logger.h"

NetworkServer* NetworkServer::getInstance() {
	static NetworkServer inst;
	return &inst;
}

void NetworkServer::init()
{
	if (initSocket(1234))
	{
		Logger::getInstance()->registerLog("SERVER::LISTEN ON PORT::" + std::to_string(1234));
		
		server_.onConnection = [this](const BOLTcpServer::TSocketChannelPtr& channel) {
			std::string peeraddr = channel->peeraddr();
			if (channel->isConnected()) {
				Logger::getInstance()->registerLog("SERVER::CONNECTED TO::" + peeraddr + "::CONNFD=" + std::to_string(channel->fd()));
				addChannel(channel);
			}
			else {
				Logger::getInstance()->registerLog("SERVER::DISCONNECTED TO::" + peeraddr + "::CONNFD=" + std::to_string(channel->fd()));
				deleteChannel(channel);
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

void NetworkServer::sendPacket(uint32_t id_channel, const Packet& packet)
{
	PacketWriter writer;
	if (findChannel(id_channel))
	{
		if (auto client = channel_map_[id_channel].lock(); client != nullptr)
		{
			auto s_packet = writer.serialize(packet);
			client->write(s_packet.data(), (int)s_packet.size());
			Logger::getInstance()->registerLog("SERVER::NUMBER OF BYTES SENT::   " + std::to_string(s_packet.size()));
		}
	}
}

void NetworkServer::sendPacketAllClients(const Packet& packet)
{
	for(const auto& [id_channel, weak_client] : channel_map_)
	{
		sendPacket(id_channel, packet);
		Logger::getInstance()->registerLog("SERVER::SEND PACKET::ID CHANNEL::" + std::to_string(id_channel));
	}
}

void NetworkServer::addChannel(const BOLTcpServer::TSocketChannelPtr& channel)
{
	std::lock_guard<std::mutex> lock(m_);
	const std::weak_ptr<BOLSocketChannel> wp(channel);
	channel_map_.emplace(channel->id_, wp);
}

void NetworkServer::deleteChannel(const BOLTcpServer::TSocketChannelPtr& channel)
{
	std::lock_guard<std::mutex> lock(m_);
	channel_map_.erase(channel->id());

	auto msg = std::make_shared<ConnectionMessage>(PacketType::MSG_DISABLE, channel->id());
	client_packet::PacketWithIdChannel pt_msg(msg, msg->id_channel);
	queue_->pushPacket(pt_msg);
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

void NetworkServer::stop()
{
	server_.stop();
}