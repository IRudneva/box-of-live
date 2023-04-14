#pragma once
#include "bol_socket_channel.h"
#include "packet_writer.h"
#include "shared_packet_queue.h"
#include "bol_timer.h"

using BOLTcpClient = hv::TcpClientTmpl<BOLSocketChannel>;

class NetworkClient
{
public:
	static NetworkClient* getInstance();

	void initQueue(std::shared_ptr<SharedPacketQueue<std::shared_ptr<Packet>>> queue) { queue_ = queue; }

	void run();

	void sendPacket(const client_packet::ClientPacket& packet);

	void stop() { client_.stop(); }

	bool checkChannelIsValid();

private:
	std::mutex m_;
	BOLTcpClient client_;
	std::shared_ptr<SharedPacketQueue<std::shared_ptr<Packet>>> queue_;
	BOLTcpClient::TSocketChannelPtr channel_ = nullptr;

	Timer count_byte_timer_{ std::chrono::milliseconds(1000) };
	size_t count_bytes_ = 0;

	NetworkClient() = default;
	~NetworkClient() = default;
	NetworkClient(const NetworkClient&) = delete;
	NetworkClient& operator=(NetworkClient&) = delete;

	friend class NetworkClientDestroyer;

	void setReconnect();

	void linkChannel(const BOLTcpClient::TSocketChannelPtr& channel);

	void unlinkChannel();

	bool initSocket(int port);
};

