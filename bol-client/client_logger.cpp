#include "pch_client.h"
#include "client_logger.h"


ClientLogger* ClientLogger::getInstance()
{
	static ClientLogger inst;
	return &inst;
}

void ClientLogger::init(tgui::ChatBox::Ptr log_box)
{
	std::weak_ptr<tgui::ChatBox> weak_box(log_box);
	log_box_ = std::move(weak_box);
}

void ClientLogger::registerLog(const std::string& log)
{
	if (log.empty())
		return;
	std::lock_guard<std::mutex> lock(m_);

	std::string str = getCurrentDatetime() + "::" + log;
	if (auto lb = log_box_.lock(); lb != nullptr)
		lb->addLine(str, tgui::Color::White);
}