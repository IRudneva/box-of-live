#pragma once
#include "ui_scene.h"
#include "cli_shared_packet_queue.h"
#include "hv/TcpClient.h"

class ClientManager
{
public:
	void initGameUI(const hv::SocketChannelPtr& channel);

	void handleUIEvent(UIEventType event, const hv::SocketChannelPtr& channel) const;

	void handlePacket(std::shared_ptr<DeserializePacket> packet, const hv::SocketChannelPtr& channel) const;
private:
	std::shared_ptr<GraphicScene> graphic_scene_;
	std::shared_ptr<SharedPacketQueue> packet_queue_ = std::make_shared<SharedPacketQueue>();;
	
	sf::RenderWindow createWindow() const;
};

