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

	void sendPacket(uint32_t id_channel, const Packet& packet);

	void sendPacketAllClients(const Packet& packet);

	int getConnectionCount() const { return channel_map_.size(); }

	void stop();

private:
	static NetworkServer* p_instance;
	static NetworkServerDestroyer destroyer;

	using IdChannel = uint32_t;

	std::mutex m_;
	std::map<IdChannel, std::weak_ptr<BOLSocketChannel>> channel_map_;
	BOLTcpServer server_;
	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> queue_;

	NetworkServer(const NetworkServer&) = delete;
	NetworkServer& operator=(NetworkServer&) =delete;
	NetworkServer() = default;
	~NetworkServer() = default;
	friend class NetworkServerDestroyer;

	bool initSocket(int port);

	void addChannel(const BOLTcpServer::TSocketChannelPtr& channel);

	void deleteChannel(const BOLTcpServer::TSocketChannelPtr& channel);

	bool findChannel(IdChannel id_channel);
};