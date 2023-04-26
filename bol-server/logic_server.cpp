#include "pch_server.h"
#include "logic_server.h"

#include "logger.h"

void LogicServer::runLogicLoop()
{
	thread_queue_is_run_ = true;
	thread_queue_ = std::thread(std::bind(&LogicServer::handleQueue, this));


	thread_db_is_run_ = true;
	thread_db_ = std::thread(std::bind(&LogicServer::handleDatabase, this));

}

void LogicServer::stopLogicLoop()
{
	thread_queue_is_run_ = false;
	thread_db_is_run_ = false;
	thread_queue_.join();
	thread_db_.join();
}

void LogicServer::handleQueue()
{
	while (thread_queue_is_run_)
	{
		if (queue_->hasPacket())
		{
			SrvManager::getInstance()->handlePacket(queue_->popPacket());
		}
		if (timer_for_game_.timedOut())
		{
			SrvManager::getInstance()->updateGameState();
		}
	}
}

void  LogicServer::handleDatabase() 
{
	while (thread_db_is_run_)
	{
		if (timer_for_save_data_.timedOut())
		{
			DbPayload::getInstance()->save();

			Logger::getInstance()->registerLog("__________SERVER::DATABASE::SAVE__________");
		}
	}
}