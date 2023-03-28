#include "pch_client.h"

#include <future>

#include "logic_client.h"
#include "network_client.h"

int main()
{
	std::shared_ptr<SharedPacketQueue<std::shared_ptr<ServerPacket>>> shared_packet_queue = std::make_shared<SharedPacketQueue<std::shared_ptr<ServerPacket>>>();
	LogicClient client(shared_packet_queue);
	client.runLoop();

	NetworkClient& network_client = NetworkClient::getInstance();
	NetworkClient::getInstance().initQueue(shared_packet_queue);
	network_client.run();
	if (getchar() == '\n')
	{
		client.stopLoop();
		network_client.stop();
	}
	return 0;
}