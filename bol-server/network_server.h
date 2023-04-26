#pragma once
#include "bol_socket_channel.h"
#include "packet_writer.h"
#include "server_packet.h"
#include "shared_packet_queue.h"
#include "bol_timer.h"
#include <map>

using BOLTcpServer = hv::TcpServerTmpl<BOLSocketChannel>;

class NetworkServer
{
public:
	static NetworkServer* getInstance();

	void init();

	void initQueue(std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue) { queue_ = queue; }

	void sendPacket(uint32_t id_channel, const Packet& packet);

	void sendPacketAllClients(const Packet& packet);

	int getConnectionCount()
	{
		std::lock_guard<std::mutex> lock(m_);
		return static_cast<int>(channel_map_.size());
	}

	void stop();

private:
	using IdChannel = uint32_t;

	std::map<IdChannel, std::weak_ptr<BOLSocketChannel>> channel_map_;
	BOLTcpServer server_;
	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue_;
	std::mutex m_;

	Timer count_byte_timer_{ std::chrono::milliseconds(1000) };
	size_t count_bytes_ = 0;

	NetworkServer(const NetworkServer&) = delete;
	NetworkServer& operator=(NetworkServer&) = delete;
	NetworkServer() = default;
	~NetworkServer() = default;

	bool initSocket(int port);

	void addChannel(const BOLTcpServer::TSocketChannelPtr& channel);

	void deleteChannel(const BOLTcpServer::TSocketChannelPtr& channel);

	bool findChannel(IdChannel id_channel);
};