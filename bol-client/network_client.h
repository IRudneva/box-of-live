#pragma once
#include "bol_socket_channel.h"
#include "packet_writer.h"
#include "shared_packet_queue.h"

using BOLTcpClient = hv::TcpClientTmpl<BOLSocketChannel>;

class NetworkClient;

class NetworkClientDestroyer
{
private:
	NetworkClient* p_instance;
public:
	~NetworkClientDestroyer();
	void initialize(NetworkClient* p);
};

class NetworkClient
{
public:
	static NetworkClient& getInstance();

	void initQueue(std::shared_ptr<SharedPacketQueue<std::shared_ptr<ServerPacket>>> queue) { queue_ = queue; }

	void run();

	void sendPacket(std::shared_ptr<ClientPacket> packet);

	void stop() { client_.stop(); }

private:
	static NetworkClient* p_instance;
	static NetworkClientDestroyer destroyer;
	BOLTcpClient client_;
	std::shared_ptr<SharedPacketQueue<std::shared_ptr<ServerPacket>>> queue_;
	BOLTcpClient::TSocketChannelPtr channel_ = nullptr;
	std::mutex m_;

	NetworkClient() = default;
	NetworkClient(const NetworkClient&) = delete;
	NetworkClient& operator=(NetworkClient&) = delete;
	~NetworkClient() = default;
	friend class NetworkClientDestroyer;

	void linkChannel(const BOLTcpClient::TSocketChannelPtr& channel);

	void unlinkChannel();

	bool checkChannelIsValid();

	bool initSocket(int port);
};

