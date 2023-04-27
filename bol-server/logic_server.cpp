#include "pch_server.h"
#include "logic_server.h"

#include "logger.h"

void LogicServer::runLogicLoop()
{
	thread_db_is_run_ = true;
	thread_db_ = std::thread(std::bind(&LogicServer::handleDatabase, this));

	thread_queue_is_run_ = true;
	thread_queue_ = std::thread(std::bind(&LogicServer::handleQueue, this));

}

void LogicServer::stopLogicLoop()
{
	thread_db_is_run_ = false;
	thread_queue_is_run_ = false;
	thread_db_.join();
	thread_queue_.join();
}

void LogicServer::handleQueue()
{
	//2й поток
	while (thread_queue_is_run_)
	{
		if (queue_->hasPacket())
		{
			manager_->handlePacket(queue_->popPacket());
		}
		if (timer_for_game_.timedOut())
		{
			manager_->updateGameState();
		}
	}
}

void LogicServer::handleDatabase() 
{
	//1й поток
	while (thread_db_is_run_)
	{
		if (timer_for_save_data_.timedOut())
		{
			db_handler_->saveData();
			Logger::getInstance()->registerLog("__________SERVER::DATABASE::SAVE__________");
		}
	}
}