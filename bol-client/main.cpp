#include "pch_client.h"

#include <future>

#include "network_client.h"
#include "bol-client.h"

void UiLoop(LogicClient& l_client)
{
	l_client.runUILoop();
}

void LogicLoop(const NetworkClient& n_client, LogicClient& l_client, std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue)
{
	while (true)
	{
		l_client.run(queue);
	}
}

int main()
{
	std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> shared_packet_queue = std::make_shared<SharedPacketQueue<DeserializePacketWithIdChannel>>();
	NetworkClient network_client(shared_packet_queue);
	network_client.run();
	LogicClient client(network_client.getIdChannel());
	std::future<void> f1 = std::async(std::launch::async, [&network_client, &client, &shared_packet_queue] {return LogicLoop(network_client, client, shared_packet_queue); });
	std::future<void> f2 = std::async(std::launch::async, [&client] {return UiLoop(client); });
	f1.get();
	f2.get();
	return 0;
}