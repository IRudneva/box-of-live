#pragma once
#include <deque>
#include <mutex>
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

	void registerLog(const std::string& log)
	{
		if (log.empty())
			return;
		std::lock_guard<std::mutex> lock(m_);

		std::string str = getCurrentDatetime() + "::" + log;
		if(auto lb = log_box_.lock(); lb !=nullptr)
			lb->addLine(str, tgui::Color::White);
	}

private:
	std::mutex m_;
	std::weak_ptr<tgui::ChatBox> log_box_;

	ClientLogger() = default;
	~ClientLogger() = default;
	ClientLogger(const  ClientLogger&) = delete;
	ClientLogger& operator=(ClientLogger&) = delete;
};

