#include "pch_server.h"
#include "logic_server.h"

void LogicServer::runLogicLoop()
{
	is_run_ = true;
	thread_ = std::thread(std::bind(&LogicServer::handleQueue, this));
}

void LogicServer::stopLogicLoop()
{
	is_run_ = false;
	thread_.join();
}

void LogicServer::handleQueue() const
{
	while (is_run_)
	{
		if (queue_->hasPacket())
		{
			srv_manager_->handlePacket(queue_->popPacket());
		}
	}
}

