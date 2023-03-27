#pragma once
#include "bol-socket-channel.h"
#include "packet_writer.h"
#include "server_packet.h"
#include "shared_packet_queue.h"
#include <map>

using BOLTcpServer = hv::TcpServerTmpl<BOLSocketChannel>;

class NetworkServer
{
public:
	explicit NetworkServer(std::shared_ptr<SharedPacketQueue<PacketWithIdChannel>> queue) :queue_(queue) {}

	void run();

	static void sendPacket(uint32_t id_channel, const ServerPacket& packet)
	{
		//проверить, подключен ли клиент
		if(findChannel(id_channel))
		{
			if(auto client = channel_map_[id_channel].lock())
			{
				PacketWriter<ServerPacket> writer;
				auto s_packet = writer.serialize(packet);
			//	auto network_packet = s_packet.packet;
				client->write((uint8_t*)&s_packet, (int)s_packet.size());
				//client->write((uint8_t*)&network_packet->header, (int)sizeof(network_packet->header));
				//client->write(network_packet->data.data(), (int)network_packet->data.size());
			}
		}
	}

	bool isRunning() const { return true; }

private:
	static inline std::map<uint32_t, std::weak_ptr<BOLSocketChannel>> channel_map_;
	static inline std::mutex m_;
	BOLTcpServer server_;
	std::shared_ptr<SharedPacketQueue<PacketWithIdChannel>> queue_;

	bool initSocket(int port);

	//std::shared_ptr<DeserializePacketWithIdChannel> pack(const BOLTcpServer::TSocketChannelPtr& channel) const;

	static void addChannel(const BOLTcpServer::TSocketChannelPtr& channel)
	{
		std::lock_guard<std::mutex> lock(m_);
		std::weak_ptr<BOLSocketChannel> wp(channel);
		channel_map_[channel->id()] = wp;
	}

	static bool findChannel(uint32_t id_channel)
	{
		std::lock_guard<std::mutex> lock(m_);
		auto client_it = channel_map_.find(id_channel);
		if (client_it != channel_map_.end())
			return true;
		return false;
	}
};