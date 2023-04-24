#include "pch_server.h"

#include "logic_server.h"
#include "shared_packet_queue.h"
#include "network_server.h"
#include "bol_database.h"
#include "request_handler.h"

int main()
{
	std::shared_ptr<SharedPacketQueue<client_packet::PacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<client_packet::PacketWithIdChannel>>();

	const database db("bol_database.db");
	std::shared_ptr<DatabaseHandler> db_handler = std::make_shared<DatabaseHandler>(db);
	RequestHandler::getInstance()->init(db_handler);
	
	LogicServer server(shared_packet_queue, db_handler);
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
