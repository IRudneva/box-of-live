#pragma once
#include "bol_socket_channel.h"
#include "packet_writer.h"
#include "server_packet.h"
#include "shared_packet_queue.h"
#include <map>

using BOLTcpServer = hv::TcpServerTmpl<BOLSocketChannel>;

class NetworkServer;

class NetworkServerDestroyer
{
private:
	NetworkServer* p_instance;
public:
	~NetworkServerDestroyer();
	void initialize(NetworkServer* p);
};

class NetworkServer
{
public:
	static NetworkServer& getInstance();

	void init();

	void initQueue(std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue) { queue_ = queue; }

	void sendPacket(uint32_t id_channel, const server_packet::ServerPacket& packet);

	void stop();

	void start();

private:
	static NetworkServer* p_instance;
	static NetworkServerDestroyer destroyer;
	std::map<uint32_t, std::weak_ptr<BOLSocketChannel>> channel_map_;
	std::mutex m_;
	BOLTcpServer server_;
	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue_;

	NetworkServer(const NetworkServer&) = delete;
	NetworkServer& operator=(NetworkServer&) =delete;
	~NetworkServer() = default;
	friend class NetworkServerDestroyer;
	NetworkServer() = default;

	bool initSocket(int port);

	void addChannel(const BOLTcpServer::TSocketChannelPtr& channel);

	void deleteChannel(const BOLTcpServer::TSocketChannelPtr& channel);

	bool findChannel(uint32_t id_channel);
};