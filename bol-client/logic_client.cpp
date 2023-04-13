#include "pch_client.h"
#include "logic_client.h"

#include "network_client.h"
#include "log_duration.h"

void LogicClient::initGraphicScene()
{
	graphic_scene_ = std::make_unique<GraphicScene>(window_);
	graphic_scene_->initGraphicScene();
}

void LogicClient::updateGraphicScene()
{
	while (window_.isOpen())
	{
		if (update_state_timer_.timedOut())
		{
			LOG_DURATION("update LOGIC_CLIENT");

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
}

void LogicClient::handlePacket(std::shared_ptr<Packet> packet) const
{
	switch (packet->type)
	{
	case PacketType::SRV_NEW_ROOM: 
	{
		auto pckt = std::static_pointer_cast<server_packet::PTNewRoom>(packet);			
		graphic_scene_->createRoom(static_cast<int>(pckt->room.id), pckt->room.name, *pckt->room.config);
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
		graphic_scene_->onNetworkConnect();
		break;
	}
	case PacketType::MSG_DISABLE:
	{
		graphic_scene_->onNetworkDisconnect();
		break;
	}
	case PacketType::SRV_INIT_CHOOSE_ROOM:
	{
		auto pckt = std::static_pointer_cast<server_packet::PTInitChooseRoom>(packet);
		graphic_scene_->onChooseRoom(*pckt->config);
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
		auto pckt = std::static_pointer_cast<server_packet::PTCloseRoom>(packet);
		graphic_scene_->onCloseRoom(static_cast<int>(pckt->id_room));
		break;
	}
	default:
		break;
	}
}