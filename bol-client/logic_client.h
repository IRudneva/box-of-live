#pragma once
#include "packet_reader.h"
#include "shared_packet_queue.h"
#include "ui_scene.h"
#include "game_domain.h"

class LogicClient
{
public:
	explicit LogicClient(std::shared_ptr<SharedPacketQueue<std::shared_ptr<server_packet::ServerPacket>>> queue) : queue_(queue) {}

	void initGraphicScene();

	void updateGameScene();

private:
	std::shared_ptr<SharedPacketQueue<std::shared_ptr<server_packet::ServerPacket>>> queue_;
	std::unique_ptr<GraphicScene> graphic_scene_;
	sf::RenderWindow window_{ {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live" };

	void handlePacket(std::shared_ptr<server_packet::ServerPacket> packet) const;
};