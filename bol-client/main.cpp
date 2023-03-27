#include "pch_client.h"

#include <future>

#include "ligic_client.h"
#include "network_client.h"

int main()
{
	std::shared_ptr<SharedPacketQueue<ServerPacket>> shared_packet_queue = std::make_shared<SharedPacketQueue<ServerPacket>>();
	LogicClient client(shared_packet_queue);
	client.runLoop();
	NetworkClient network_client(shared_packet_queue);
	network_client.run();
	client.stopLoop();
	return 0;
}