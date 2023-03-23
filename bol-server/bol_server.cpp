#include "pch_server.h"
#include "bol_server.h"

#include <iostream>

void LogicServer::run(std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue)
{
	if(queue->hasPacket())
	{
		srv_manager_->handlePacket(queue->popPacket());
	}

}
