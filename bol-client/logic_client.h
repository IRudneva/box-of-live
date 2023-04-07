#pragma once
#include "server_packet.h"
#include "shared_packet_queue.h"
#include "graphic_scene.h"
#include "game_domain.h"

class LogicClient
{
public:
	explicit LogicClient(std::shared_ptr<SharedPacketQueue<std::shared_ptr<Packet>>> queue) : queue_(queue) {}

	void initGraphicScene();

	void updateGraphicScene();

private:
	std::shared_ptr<SharedPacketQueue<std::shared_ptr<Packet>>> queue_;
	std::unique_ptr<GraphicScene> graphic_scene_;
	sf::RenderWindow window_{ {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live" };

	void handlePacket(std::shared_ptr<Packet> packet) const;
};