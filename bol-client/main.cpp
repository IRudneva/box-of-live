#include "pch_client.h"
#include "logic_client.h"
#include "network_client.h"

int main()
{
	std::shared_ptr<SharedPacketQueue<std::shared_ptr<Packet>>> shared_packet_queue = std::make_shared<SharedPacketQueue<std::shared_ptr<Packet>>>();
	LogicClient logic_client(shared_packet_queue);
	logic_client.initGraphicScene();

	NetworkClient& network_client = NetworkClient::getInstance();
	NetworkClient::getInstance().initQueue(shared_packet_queue);

	network_client.run();
	logic_client.updateGameScene();
	network_client.stop();
	return 0;
}