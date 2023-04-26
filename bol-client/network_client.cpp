#include "pch_client.h"
#include "network_client.h"
#include "client_logger.h"

NetworkClient*  NetworkClient::getInstance() {
	static NetworkClient inst;
	return &inst;
}

void NetworkClient::run()
{
	if (initSocket(1234))
	{
		client_.onConnection = [&](const BOLTcpClient::TSocketChannelPtr& channel) {

			std::string peeraddr = channel->peeraddr();
			if (channel->isConnected()) {
				ClientLogger::getInstance()->registerLog("CLIENT::CONNECTED TO::" + peeraddr + "::CONNFD=" + std::to_string(channel->fd()));
				linkChannel(channel);
			}
			else {
				ClientLogger::getInstance()->registerLog("CLIENT::DISCONNECTED TO::" + peeraddr + "::CONNFD=" + std::to_string(channel->fd()));
				unlinkChannel();
			}
			if (client_.isReconnect()) {
				ClientLogger::getInstance()->registerLog("CLIENT::RECONNECT::CNT=" + std::to_string(client_.reconn_setting->cur_retry_cnt) + "::DELAY=" + std::to_string(client_.reconn_setting->cur_delay));
			}
		};

		client_.onMessage = [this](const BOLTcpClient::TSocketChannelPtr& channel, hv::Buffer* buf) {
			uint8_t* it = (uint8_t*)buf->data();
			size_t size_left = buf->size();
			count_bytes_ += buf->size();
			if (count_byte_timer_.timedOut())
			{
				ClientLogger::getInstance()->registerLog("CLIENT::RECEIVED BYTES PER SECOND::   " + std::to_string(count_bytes_));
				count_bytes_ = 0;
			}
			do {
				size_left = channel->reader_.readNetworkPacket(&it, size_left);
				if (channel->reader_.isAllDataComplete()) {
					auto type = channel->reader_.getPacketType();
					auto data = channel->reader_.getData();
					auto packet = ServerPacketBuilder::getPacket(type, data);
					queue_->pushPacket(packet);
				
				}
			} while (size_left > 0);

		};

		setReconnect();
		client_.start();
	}
}

void NetworkClient::setReconnect()
{
	// reconnect: 1,2,4,8,10,10,10...
	reconn_setting_t reconn;
	reconn_setting_init(&reconn);
	reconn.min_delay = 1000;
	reconn.max_delay = 10000;
	reconn.delay_policy = 2;
	client_.setReconnect(&reconn);
}


bool NetworkClient::initSocket(int port)
{
	int connfd = client_.createsocket(port);
	if (connfd < 0) {
		return false;
	}
	ClientLogger::getInstance()->registerLog("CLIENT::CONNECTED TO PORT::" + std::to_string(port) + "::CONNFD=" + std::to_string(connfd));
	return true;
}

void NetworkClient::sendPacket(const client_packet::ClientPacket& packet)
{
	if (!checkChannelIsValid())
		return;
	std::lock_guard<std::mutex> lock(m_);
	PacketWriter writer;
	std::vector<uint8_t> s_packet = writer.serialize(packet);
	channel_->write(s_packet.data(), (int)s_packet.size());
	ClientLogger::getInstance()->registerLog("CLIENT::SEND PACKET::SIZE   " + std::to_string(s_packet.size()));
}

void NetworkClient::linkChannel(const BOLTcpClient::TSocketChannelPtr& channel)
{
	std::lock_guard<std::mutex> lock(m_);
	channel_ = channel;
	auto message = std::make_shared<ConnectionMessage>(PacketType::MSG_CONNECTED);
	queue_->pushPacket(message);
}

void NetworkClient::unlinkChannel()
{
	std::lock_guard<std::mutex> lock(m_);
	channel_ = nullptr;
	auto message = std::make_shared<ConnectionMessage>(PacketType::MSG_DISABLE);
	queue_->pushPacket(message);
}

bool NetworkClient::checkChannelIsValid()
{
	std::lock_guard<std::mutex> lock(m_);
	if (channel_ == nullptr)
		return false;
	return true;
}