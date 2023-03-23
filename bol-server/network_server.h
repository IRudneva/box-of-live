#pragma once
#include "bol-socket-channel.h"
#include "deserialize_packet.h"
#include "srv_shared_packet_queue.h"
#include <map>

using BOLTcpServer = hv::TcpServerTmpl<BOLSocketChannel>;

class NetworkServer
{
public:
	explicit NetworkServer(std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue) :queue_(queue) {}

	void run();

	static void sendPacket(const SerializedPacketWithIdChannel& s_packet)
	{
		//проверить, подключен ли клиент
		auto client_it = channel_map_.find(s_packet.id_channel);
		if(client_it != channel_map_.end())
		{
			if(auto client = client_it->second.lock())
			{
				auto network_packet = s_packet.packet;
				client->write((uint8_t*)&network_packet->header, (int)sizeof(network_packet->header));
				client->write(network_packet->data.data(), (int)network_packet->data.size());
			}
		}
	}

	bool isRunning() const { return true; }

private:
	static inline std::map<uint32_t, std::weak_ptr<BOLSocketChannel>> channel_map_;
	BOLTcpServer server_;
	std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue_;

	bool initSocket(int port);

	std::shared_ptr<DeserializePacketWithIdChannel> pack(const BOLTcpServer::TSocketChannelPtr& channel) const;
};