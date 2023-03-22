#include "pch_server.h"
#include "bol_server.h"
#include "srv_shared_packet_queue.h"
#include "network_server.h"


int main()
{
	std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<DeserializePacketWithIdChannel>>();
	NetworkServer networck_server(shared_packet_queue);
	LogicServer server;
	server.run(shared_packet_queue);
		/*std::string str;
		while (std::getline(std::cin, str)) {
			if (str == "close") {
				server_.closesocket();
			}
			else if (str == "start") {
				server_.start();
			}
			else if (str == "stop") {
				server_.stop();
				break;
			}
			else {
				server_.broadcast(str.data(), str.size());
			}
		}*/
	
	return 0;
}