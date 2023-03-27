#include "pch_client.h"
#include "ligic_client.h"

void LogicClient::runLoop()
{
	ui_is_run_ = true;
	ui_thread_ = std::thread(std::bind(&LogicClient::updateUIScene, this));
	logic_is_run_ = true;
	logic_thread_ = std::thread(std::bind(&LogicClient::handleQueue, this));
}

void LogicClient::stopLoop()
{
	logic_is_run_ = false;
	ui_is_run_ = false;
	logic_thread_.join();
	ui_thread_.join();
}


