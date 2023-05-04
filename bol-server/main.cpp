#include "pch_server.h"

#include "logic_server.h"
#include "shared_packet_queue.h"
#include "network_server.h"
#include "db_handler.h"

int main()
{
	Logger::getInstance()->registerLog("PORT: ");
	std::string port;
	std::getline(std::cin, port);
	if (stoi(port) < 1025) {
		return -10;
	}

	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<client_packet::PacketWithIdChannel>>();
	
	NetworkServer::getInstance()->initQueue(shared_packet_queue);
	NetworkServer::getInstance()->init(stoi(port));

	LogicServer server(shared_packet_queue);
	server.runLogicLoop();

	if(getchar() == '\n')
	{
		server.stopLogicLoop();
		NetworkServer::getInstance()->stop();
	}
	return 0;
}
