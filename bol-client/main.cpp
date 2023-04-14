#include "pch_client.h"
#include "logic_client.h"
#include "network_client.h"

int WINAPI WinMain(HINSTANCE hInstatnce, HINSTANCE prevInstance, PSTR lpCmdLine, int nCmdShow)
{
	std::shared_ptr<SharedPacketQueue<std::shared_ptr<Packet>>> shared_packet_queue = std::make_shared<SharedPacketQueue<std::shared_ptr<Packet>>>();
	LogicClient logic_client(shared_packet_queue);
	logic_client.initGraphicScene();

	NetworkClient::getInstance()->initQueue(shared_packet_queue);

	NetworkClient::getInstance()->run();
	logic_client.updateGraphicScene();
	NetworkClient::getInstance()->stop();
	return 0;
}