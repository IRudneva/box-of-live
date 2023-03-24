#pragma once
#include "bol_socket_channel.h"
#include "deserialize_packet.h"
#include "shared_packet_queue.h"

using BOLTcpClient = hv::TcpClientTmpl<BOLSocketChannel>;

class NetworkClient
{
public:
	NetworkClient(std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue) : queue_(queue) {}

	void run();

	static void sendPacket(const SerializedPacketWithIdChannel& s_packet)
	{
		if (s_packet.id_channel != channel_->id() || channel_ == nullptr)
			return;
		auto network_packet = s_packet.packet;
		channel_->write((uint8_t*)&network_packet->header, (int)sizeof(network_packet->header));
		channel_->write(network_packet->data.data(), (int)network_packet->data.size());
		std::cout << "send packet!" << std::endl;
	}

	uint32_t getIdChannel() const
	{
		if (channel_ != nullptr)
			return channel_->id();
	}

private:
	BOLTcpClient client_;
	std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue_;
	static inline BOLTcpClient::TSocketChannelPtr channel_ = nullptr;
	bool initSocket(int port);

	std::shared_ptr<DeserializePacketWithIdChannel> pack(const BOLTcpClient::TSocketChannelPtr& channel) const;
};

