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
	thread_queue_is_run_ = false;
	thread_db_is_run_ = false;
	thread_queue_.join();
	thread_db_.join();
}

void LogicServer::handleQueue() const
{
	while (thread_queue_is_run_)
	{
		if (queue_->hasPacket())
		{
			srv_manager_->handlePacket(queue_->popPacket());
		}
		srv_manager_->updateGameState();
	}
}

void  LogicServer::handleDatabase() 
{
	while (thread_db_is_run_)
	{
		if (timer_for_save_data_.timedOut())
		{
			db_handler_->saveConfig(srv_manager_->formConfigForDatabase());

			db_handler_->saveFieldsState(srv_manager_->formFieldsStateForDatabase());

			db_handler_->saveBacteruium(srv_manager_->formBacteriumInfoForDatabase());

			Logger::getInstance()->registerLog("__________SERVER::DATABASE::SAVE__________");
		}
		
	}
}