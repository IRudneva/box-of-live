#include "pch_client.h"
#include "cli_manager.h"
#include "game_domain.h"
#include "network_client.h"
#include "packet_writer.h"
#include "ui_scene.h"

void ClientManager::updateGameScene()
{
	auto window = createWindow();
	auto gr_scene = std::make_shared<GraphicScene>(window);
	graphic_scene_ = std::move(gr_scene);
	graphic_scene_->init();

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			graphic_scene_->handleEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (graphic_scene_->isPressedCreateRoomButton())
				{
					handleUIEvent(UIEventType::PRESSED_BUTTON_CREATE_ROOM);
				}
				if (graphic_scene_->isPressedChooseRoomButton())
				{
					handleUIEvent(UIEventType::PRESSED_BUTTON_CHOOSE_ROOM);
				}
			}
		}

		window.clear();
		graphic_scene_->drawGui();
		window.display();
	}
}

void ClientManager::handleUIEvent(UIEventType event) const
{
	switch (event)
	{
	case UIEventType::PRESSED_BUTTON_CREATE_ROOM:
	{
		std::cout << "press BCreateR" << std::endl;
		std::shared_ptr<PTCreateRoom> packet = std::make_shared<PTCreateRoom>("room ololo");
		
		NetworkClient::getInstance().sendPacket(packet);
		{ // TEST
			/*std::string test;
			char t = '*';
			for (auto i = 0; i < 1000000; i++)
			{
				test.push_back(t);
			}
			std::shared_ptr<PTCreateRoom> pt_create_room = std::make_shared<PTCreateRoom>(test);
			ds_packet.packet = std::static_pointer_cast<DeserializePacket>(pt_create_room);
			ds_packet.id_channel = id_channel_;

			auto s_packet = writer.getSerializePacket(ds_packet);

			NetworkClient::sendPacket(s_packet);*/
		}
	
		break;
	}
	case UIEventType::PRESSED_BUTTON_CHOOSE_ROOM:
	{
		std::cout << "press BChooseR" << std::endl;
		std::shared_ptr<PTGetRoomList> pt_ger_room_list = std::make_shared<PTGetRoomList>();
		NetworkClient::getInstance().sendPacket(pt_ger_room_list);
		break;
	}
	case UIEventType::NO_EVENT: 
		break;
	default:
		break;
	}
}

void ClientManager::handlePacket(std::shared_ptr<ServerPacket> packet) const
{
	switch (packet->type)
	{
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

sf::RenderWindow ClientManager::createWindow() const { return { {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live" }; }