#include "pch_client.h"
#include "logic_client.h"

#include "network_client.h"

void LogicClient::initGraphicScene()
{
	graphic_scene_ = std::make_unique<GraphicScene>(window_);
	graphic_scene_->initGraphicScene();
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
		auto pt_new_room = std::static_pointer_cast<server_packet::PTNewRoom>(packet);
		std::cout << "i received PTCreateRoom "  << pt_new_room->room.id << std::endl;
			
		graphic_scene_->createRoom(static_cast<int>(pt_new_room->room.id), pt_new_room->room.name);
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
		graphic_scene_->initButtonStart(false);
		graphic_scene_->initButtonCloseRoom(false);
		graphic_scene_->clearRoomList();
	//	graphic_scene_->backToMenu();
		break;
	}
	case PacketType::SRV_START_GAME:
	{
		std::cout << "i received START_GAME" << std::endl;
		//auto pt_start_game = std::static_pointer_cast<server_packet::PTStartGame>(packet);
		
		break;
	}
	case PacketType::SRV_INIT_CHOOSE_ROOM:
	{
		std::cout << "i received InitChooseRoom" << std::endl;
		auto pt_choose_room = std::static_pointer_cast<server_packet::PTInitChooseRoom>(packet);
		graphic_scene_->initButtonStart(true);
		graphic_scene_->initButtonCloseRoom(true);
		graphic_scene_->initConfigGrid(pt_choose_room->id_room, true);
		break;
	}
	case PacketType::SRV_ROOM_STATE:
	{
		auto pt_room_state = std::static_pointer_cast<server_packet::PTRoomState>(packet);
		std::cout << std::this_thread::get_id() << "thread" << std::endl;
		std::cout << "i received SRVROOM_STATE  " << pt_room_state->id_room << std::endl;
		graphic_scene_->drawGui(pt_room_state->id_room, pt_room_state->grass_info, pt_room_state->bacterium_info);
		break;
	}
	case PacketType::SRV_CLOSE_ROOM:
	{
		auto pt_close_room = std::static_pointer_cast<server_packet::PTCloseRoom>(packet);
		graphic_scene_->backToMenu(pt_close_room->id_room);
		break;
	}
	case PacketType::SRV_NEW_CONFIG:
	{
		std::cout << "i received NewConfig" << std::endl;
		auto pt_config = std::static_pointer_cast<server_packet::PTNewConfig>(packet);
		graphic_scene_->setConfigForRoom(pt_config->id_room, pt_config->game_config);
		break;
	}
	default:
		break;
	}
}