﻿#include "pch_client.h"
#include "logic_client.h"

#include "network_client.h"

void LogicClient::initGraphicScene()
{
	graphic_scene_ = std::make_unique<GraphicScene>(window_);
	graphic_scene_->init();
}

void LogicClient::updateGameScene()
{
	while (window_.isOpen())
	{
		if(!NetworkClient::getInstance().checkChannelIsValid())
			graphic_scene_->backToMenu();

		if (queue_->hasPacket())
		{
			auto packet = queue_->popPacket();
			handlePacket(packet);
		}

		sf::Event event{};
		while (window_.pollEvent(event))
		{
			graphic_scene_->handleEvent(event);

			if (event.type == sf::Event::Closed)
			{
				graphic_scene_ = nullptr;
				window_.close();
				return;
			}
		}

		window_.clear();
		graphic_scene_->drawGui();
		window_.display();
	}
}

void LogicClient::handlePacket(std::shared_ptr<ServerPacket> packet) const
{
	switch (packet->type)
	{
	case PacketType::PT_CREATE_ROOM: 
	{
		auto pt_new_room = std::static_pointer_cast<PTNewRoom>(packet);
		std::cout << "i received PTCreateRoom" << std::endl;
		graphic_scene_->createRoom(pt_new_room->room.getId(), pt_new_room->room.name);
		break;
	}
	case PacketType::PT_ROOM_LIST:
	{
		std::vector<std::string> rl;
		auto pt_room_list = std::static_pointer_cast<PTRoomList>(packet);
		std::cout << "i received PTRoomList" << std::endl;
		for (auto r : pt_room_list->room_list)
		{
			std::cout << "Room id: " << r.getId() << " room name: " << r.name << std::endl;
			rl.push_back(std::to_string(r.getId()) + " " + r.name);
		}
		graphic_scene_->createRoomList(rl);
		break;
	}
	default:
		break;
	}
}