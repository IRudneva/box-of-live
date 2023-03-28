#include "pch_server.h"
#include "logic_server.h"
#include "shared_packet_queue.h"
#include "network_server.h"

int main()
{
	std::shared_ptr<SharedPacketQueue<PacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<PacketWithIdChannel>>();
	LogicServer server(shared_packet_queue);
	server.runLogicLoop();
	
	NetworkServer& network_server = NetworkServer::getInstance();
	NetworkServer::getInstance().initQueue(shared_packet_queue);
	network_server.run();

	if (getchar() == '\n')
	{
		network_server.stop();
		server.stopLogicLoop();
	}
	return 0;
}