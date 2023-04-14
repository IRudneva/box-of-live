#pragma once
#include <deque>
#include <string>
#include "bol_timer.h"

class ClientLogger
{
public:
	static ClientLogger* getInstance()
	{
		static ClientLogger inst;
		return &inst;
	}

	void init(tgui::ChatBox::Ptr log_box) 
	{
		std::weak_ptr<tgui::ChatBox> weak_box(log_box);
		log_box_ = std::move(weak_box);
	}

	void registerLog/*Client*/(const std::string& log)
	{
		std::string str = getCurrentDatetime() + "::" + log;
		if(auto lb = log_box_.lock(); lb !=nullptr)
			lb->addLine(str, tgui::Color::White);
	}

	/*
		void registerLogServer(const std::string& log)
		{

		}*/

private:
	//std::deque<std::string> queue_;
	std::weak_ptr<tgui::ChatBox> log_box_;

	ClientLogger() = default;
	~ClientLogger() = default;
	ClientLogger(const  ClientLogger&) = delete;
	ClientLogger& operator=(ClientLogger&) = delete;
};

