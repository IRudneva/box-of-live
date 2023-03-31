#include "pch_client.h"
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

void LogicClient::handlePacket(std::shared_ptr<Packet> packet) const
{
	switch (packet->type)
	{
	case PacketType::SRV_NEW_ROOM: 
	{
		auto pt_new_room = std::static_pointer_cast<server_packet::PTNewRoom>(packet);
		std::cout << "i received PTCreateRoom "  << pt_new_room->room.id << std::endl;
			
		graphic_scene_->createRoom(pt_new_room->room.id, pt_new_room->room.name);
		break;
	}
	case PacketType::SRV_ROOM_LIST:
	{
		auto pt_room_list = std::static_pointer_cast<server_packet::PTRoomList>(packet);
		std::cout << "i received PTRoomList" << std::endl;
		graphic_scene_->createRoomList(pt_room_list->room_list);
		break;
	}
	case  PacketType::MSG_CONNECTED:
	{
		graphic_scene_->initConnectionFlag(true);
		break;
	}
	case PacketType::MSG_DISABLE:
	{
		graphic_scene_->initConnectionFlag(false);
		graphic_scene_->backToMenu();
		break;
	}
	case PacketType::SRV_ROOM_STATE:
	{
		std::cout << "i received SRVROOM_STATE" << std::endl;
		break;
	}
	case PacketType::SRV_CLOSE_ROOM:
		break;
	default:
		break;
	}
}