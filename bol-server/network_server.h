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

	static void sendPacket(/*const BOLTcpServer::TSocketChannelPtr& channel, */const SerializedPacketWithIdChannel& s_packet)
	{
		//проверить, подключен ли клиент
		auto client_it = channel_map_.find(s_packet.id_channel);
		if(client_it != channel_map_.end())
		{
			if(auto client = client_it->second.lock())
			{
				auto network_packet = s_packet.packet;
				client->get()->write((uint8_t*)&network_packet->header, (int)sizeof(network_packet->header));
				client->get()->write(network_packet->data.data(), (int)network_packet->data.size());
			}
		}
		/*channel->write((uint8_t*)&packet->header, (int)sizeof(packet->header));
		channel->write(packet->data.data(), (int)packet->data.size());*/
	}

	/*static inline std::map<uint32_t, std::weak_ptr<BOLTcpServer::TSocketChannelPtr>> channel_map_;*/
private:
	static inline std::map<uint32_t, std::weak_ptr<BOLTcpServer::TSocketChannelPtr>> channel_map_;
	BOLTcpServer server_;
	std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue_;

	bool initSocket(int port);

	DeserializePacketWithIdChannel pack(const BOLTcpServer::TSocketChannelPtr& channel) const;
};