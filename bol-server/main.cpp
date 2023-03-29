#include "pch_server.h"

#include <iostream>

#include "logic_server.h"
#include "shared_packet_queue.h"
#include "network_server.h"

int main()
{
	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<client_packet::PacketWithIdChannel>>();
	LogicServer server(shared_packet_queue);
	server.runLogicLoop();
	
	NetworkServer& network_server = NetworkServer::getInstance();
	NetworkServer::getInstance().initQueue(shared_packet_queue);
	network_server.init();

	if(getchar() == '\n')
	{
		server.stopLogicLoop();
		network_server.stop();
	}
	return 0;
}