#include "pch_client.h"
#include "logic_client.h"

#include "network_client.h"

void LogicClient::initGraphicScene()
{
	graphic_scene_ = std::make_unique<GraphicScene>(window_);
	graphic_scene_->initGraphicScene();
}

void LogicClient::updateGraphicScene()
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
		graphic_scene_->update();
		window_.display();
	}
}

void LogicClient::handlePacket(std::shared_ptr<Packet> packet) const
{
	switch (packet->type)
	{
	case PacketType::SRV_NEW_ROOM: 
	{
		auto pckt = std::static_pointer_cast<server_packet::PTNewRoom>(packet);			
		graphic_scene_->createRoom(static_cast<int>(pckt->room.id), pckt->room.name);
		break;
	}
	case PacketType::SRV_ROOM_LIST:
	{
		auto pckt = std::static_pointer_cast<server_packet::PTRoomList>(packet);
		graphic_scene_->createRoomList(pckt->room_list);
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
		graphic_scene_->initButtonStart(false);
		graphic_scene_->initButtonCloseRoom(false);
		graphic_scene_->initAllConfigGrid(false);
		graphic_scene_->clearRoomList();
		graphic_scene_->clearGameCanvas();
		break;
	}
	case PacketType::SRV_INIT_CHOOSE_ROOM:
	{
		auto pckt = std::static_pointer_cast<server_packet::PTInitChooseRoom>(packet);
		graphic_scene_->initButtonStart(true);
		graphic_scene_->initButtonCloseRoom(true);
		graphic_scene_->initConfigGrid(pckt->id_room, true);
		break;
	}
	case PacketType::SRV_ROOM_STATE:
	{
		auto pckt = std::static_pointer_cast<server_packet::PTRoomState>(packet);
		graphic_scene_->drawGameCanvas(pckt->id_room, pckt->grass_info, pckt->bacterium_info);
		break;
	}
	case PacketType::SRV_CLOSE_ROOM:
	{
		auto pt_room = std::static_pointer_cast<server_packet::PTCloseRoom>(packet);
		graphic_scene_->initConfigGrid(pt_room->id_room, false);
		graphic_scene_->deleteRoom(pt_room->id_room);
		graphic_scene_->clearGameCanvas();
		graphic_scene_->initButtonStart(false);
		graphic_scene_->initButtonCloseRoom(false);
		break;
	}
	default:
		break;
	}
}