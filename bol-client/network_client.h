#pragma once
#include "bol_socket_channel.h"
#include "packet_writer.h"
#include "shared_packet_queue.h"

using BOLTcpClient = hv::TcpClientTmpl<BOLSocketChannel>;

class NetworkClient
{
public:
	NetworkClient(std::shared_ptr<SharedPacketQueue<ServerPacket>> queue) : queue_(queue) {}

	void run();

	static void sendPacket(const ClientPacket& packet)
	{
		if (!checkChannelIsValid())
			return;
		PacketWriter<ClientPacket> writer;
		auto s_packet = writer.serialize(packet);
		channel_->write((uint8_t*)&s_packet, (int)s_packet.size());
		std::cout << "send packet!" << std::endl;
	}

	uint32_t getIdChannel() const
	{
		if (channel_ != nullptr)
			return channel_->id();
	}

private:
	BOLTcpClient client_;
	std::shared_ptr<SharedPacketQueue<ServerPacket>> queue_;
	static inline BOLTcpClient::TSocketChannelPtr channel_ = nullptr;
	static inline std::mutex m_;

	static void linkChannel(const BOLTcpClient::TSocketChannelPtr& channel)
	{
		std::lock_guard<std::mutex> lock(m_);
		channel_ = channel;
	}

	static void unlinkChannel()
	{
		std::lock_guard<std::mutex> lock(m_);
		channel_ = nullptr;
	}

	static bool checkChannelIsValid(/*uint32_t id_channel*/)
	{
		std::lock_guard<std::mutex> lock(m_);
		if (channel_ == nullptr)
			return false;
		/*if (channel_->id() != id_channel)
			return false;*/
		return true;
	}

	bool initSocket(int port);


	//std::shared_ptr<DeserializePacketWithIdChannel> pack(const BOLTcpClient::TSocketChannelPtr& channel) const;
};

