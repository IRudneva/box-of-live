#include "pch_client.h"
#include "bol-client.h"


void LogicClient::run(std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue)
{
	if(queue->hasPacket())
	{
		ui_manager_->handlePacket(queue->popPacket());
	}
}


