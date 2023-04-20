#include "pch_server.h"
#include "logic_server.h"

void LogicServer::runLogicLoop()
{
	srv_manager_->initRoomList();

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
		if(timer_for_save_data_.timedOut())
		{
			srv_manager_->formDataForDatabase();

			//if(DataBuffer::getInstance()->hasData())
			//{
				db_handler_->saveData(DataBuffer::getInstance()->popData());
			//}
		}
		
	}
}