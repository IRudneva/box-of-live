#include "pch_client.h"
#include "cli_manager.h"
#include "game_domain.h"
#include "network_client.h"
#include "packet_writer.h"
#include "ui_scene.h"

void ClientManager::initGameUI()
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
		PTCreateRoom packet("room ololo");
		NetworkClient::sendPacket(packet);
		{
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
		PTGetRoomList pt_ger_room_list;
		NetworkClient::sendPacket(pt_ger_room_list);
		break;
	}
	case UIEventType::NO_EVENT: 
		break;
	default:
		break;
	}
}

void ClientManager::handlePacket(ServerPacket& packet) const
{
	switch (packet.type)
	{
	case PacketType::PT_ROOM_LIST:
	{
		ServerPacket& ppacket = packet;
		auto pt_room_list = dynamic_cast<PTRoomList&>(ppacket);
		std::cout << "i received PTRoomList" << std::endl;
		for (auto r : pt_room_list.room_list)
		{
			std::cout << "Room id: " << r.getId() << " room name: " << r.name << std::endl;
		}
		break;
	}
	default:
		break;
	}
}

sf::RenderWindow ClientManager::createWindow() const { return { {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live" }; }