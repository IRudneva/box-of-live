#pragma once
#include <deque>
#include <mutex>
#include <string>
#include "bol_timer.h"

class ClientLogger
{
public:
	static ClientLogger* getInstance();

	void init(tgui::ChatBox::Ptr log_box);

	void registerLog(const std::string& log);

private:
	std::mutex m_;
	std::weak_ptr<tgui::ChatBox> log_box_;

	ClientLogger() = default;
	~ClientLogger() = default;
	ClientLogger(const  ClientLogger&) = delete;
	ClientLogger& operator=(ClientLogger&) = delete;
};

