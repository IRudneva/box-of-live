#include "pch_server.h"

#include "logic_server.h"
#include "shared_packet_queue.h"
#include "network_server.h"
#include "db_handler.h"

int main()
{
	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<client_packet::PacketWithIdChannel>>();

	LogicServer server(shared_packet_queue);
	server.runLogicLoop();
	
	NetworkServer::getInstance()->initQueue(shared_packet_queue);
	NetworkServer::getInstance()->init();

	if(getchar() == '\n')
	{
		server.stopLogicLoop();
		NetworkServer::getInstance()->stop();
	}
	return 0;
}
