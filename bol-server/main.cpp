#include "pch_server.h"
#include "bol_server.h"
#include "shared_packet_queue.h"
#include "network_server.h"
#include <future>

//void NetworkLoop(NetworkServer& n_server)
//{
//	std::cout << "NetworkLoop thread:: " << std::this_thread::get_id() << std::endl;
//	n_server.run();
//}

void LogicLoop(const NetworkServer& n_server, LogicServer& l_server, std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue)
{
	std::cout << "LogicLoop thread:: " << std::this_thread::get_id() << std::endl;
	while(true)
	{
		l_server.run(queue);
	}
}

int main()
{
	std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<DeserializePacketWithIdChannel>>();
	NetworkServer network_server(shared_packet_queue);
	network_server.run();
	//std::future<void> f1 = std::async(std::launch::async, [&network_server] {return NetworkLoop(network_server); });
	LogicServer server;
	std::future<void> f2 = std::async(std::launch::async, [&network_server, &server, &shared_packet_queue] {return LogicLoop(network_server, server, shared_packet_queue); });
	//f1.get();
	f2.get();
	return 0;
}