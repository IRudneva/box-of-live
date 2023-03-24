#pragma once
#include "packet_reader.h"
#include "cli_manager.h"
#include "shared_packet_queue.h"


class LogicClient
{
public:
	explicit LogicClient(uint32_t id_channel) : id_channel_(id_channel) {}
	void run(std::shared_ptr<SharedPacketQueue<DeserializePacketWithIdChannel>> queue);
	void runUILoop()
	{
		ui_manager_->initGameUI();
	}
private:
	uint32_t id_channel_;
	std::unique_ptr<ClientManager> ui_manager_ = std::make_unique<ClientManager>(id_channel_);
};