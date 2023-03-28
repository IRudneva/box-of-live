#include "pch_client.h"
#include "logic_client.h"

void LogicClient::runLoop()
{
	logic_is_run_ = true;
	logic_thread_ = std::thread(std::bind(&LogicClient::handleQueue, this));
	ui_is_run_ = true;
	ui_thread_ = std::thread(std::bind(&LogicClient::updateUI, this));
}

void LogicClient::stopLoop()
{
	logic_is_run_ = false;
	ui_is_run_ = false;
	logic_thread_.join();
	ui_thread_.join();
}

void LogicClient::handleQueue() const
{
	while (logic_is_run_)
	{
		if (queue_->hasPacket())
		{
			auto packet = queue_->popPacket();
			ui_manager_->handlePacket(packet);
		}
	}
}

void LogicClient::updateUI()
{
	while (ui_is_run_)
	{
		ui_manager_->updateGameScene();
	}
}