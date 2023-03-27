#include "pch_server.h"
#include "logic_server.h"
#include "shared_packet_queue.h"
#include "network_server.h"

int main()
{
	std::shared_ptr<SharedPacketQueue<PacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<PacketWithIdChannel>>();
	LogicServer server(shared_packet_queue);
	server.runLogicLoop();
	NetworkServer network_server(shared_packet_queue);
	network_server.run();
	server.stopLogicLoop();
	return 0;
}