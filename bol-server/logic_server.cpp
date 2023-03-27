#include "pch_server.h"
#include "logic_server.h"

#include <iostream>

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


