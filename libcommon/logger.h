#pragma once
#include <deque>
#include <string>
#include <iostream>
#include "bol_timer.h"

class Logger
{
public:
	static Logger* getInstance()
	{
		static Logger inst;
		return &inst;
	}

	void registerLog/*Client*/(const std::string& log)
	{
		std::string str = getCurrentDatetime() + "::" + log;
		queue_.push_back(str);
		std::cerr << str << std::endl;
	}
/*
	void registerLogServer(const std::string& log)
	{

	}*/

private:
	std::deque<std::string> queue_;

	Logger() = default;
	~Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(Logger&) = delete;
};

