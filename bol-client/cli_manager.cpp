#include "pch_client.h"
#include "cli_manager.h"
#include "game_domain.h"
#include "network_client.h"
#include "packet_writer.h"
#include "ui_scene.h"


void ClientManager::initGameUI()
{
	auto window = createWindow();
	graphic_scene_ = std::make_shared<GraphicScene>(window);
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
	DeserializePacketWriter writer;
	switch (event)
	{
	case UIEventType::PRESSED_BUTTON_CREATE_ROOM:
	{
		std::cout << "press BCreateR" << std::endl;
		DeserializePacketWithIdChannel ds_packet;
		std::shared_ptr<PTCreateRoom> pt_create_room = std::make_shared<PTCreateRoom>("room ololo");
		ds_packet.packet = std::static_pointer_cast<DeserializePacket>(pt_create_room);
		ds_packet.id_channel = id_channel_;

		auto s_packet = writer.getSerializePacket(ds_packet);

		NetworkClient::sendPacket(s_packet);
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
		DeserializePacketWithIdChannel ds_packet;
		std::shared_ptr<PTGetRoomList> pt_ger_room_list = std::make_shared<PTGetRoomList>();
		ds_packet.packet = std::static_pointer_cast<DeserializePacket>(pt_ger_room_list);
		ds_packet.id_channel = id_channel_;
		auto s_packet = writer.getSerializePacket(ds_packet);
		NetworkClient::sendPacket(s_packet);
		break;
	}
	case UIEventType::NO_EVENT: 
		break;
	default:
		break;
	}
}

void ClientManager::handlePacket(const DeserializePacketWithIdChannel& packet) const
{
	std::shared_ptr<DeserializePacket> cur_packet = packet.packet;

	switch (cur_packet->type)
	{
	case PacketType::PT_CREATE_ROOM:
	{
		DeserializePacket& c = *cur_packet;
		auto p_p = dynamic_cast<PTCreateRoom&>(c);
		std::cout << "i received PTCreateRoom" << std::endl;

		break;
	}
	case PacketType::PT_CLOSE_ROOM:
	{
		DeserializePacket& c = *cur_packet;
		auto p_p = dynamic_cast<PTCloseRoom&>(c);
		std::cout << "i received PTCloseRoom" << std::endl;

		break;
	}
	case PacketType::PT_GET_ROOM_LIST:
	{
		DeserializePacket& c = *cur_packet;
		auto p_p = dynamic_cast<PTGetRoomList&>(c);
		std::cout << "i received PTGetRoomList" << std::endl;

		break;
	}
	case PacketType::PT_ROOM_LIST:
	{
		DeserializePacket& c = *cur_packet;
		auto p_p = dynamic_cast<PTRoomList&>(c);
		std::cout << "i received PTRoomList" << std::endl;
		for (auto r : p_p.room_list)
		{
			std::cout << "Room id: " << r.getId() << " room name: " << r.name << std::endl;
		}
		//////////////////////////////////////////////////////
		break;
	}
	default:
		break;
	}
}

sf::RenderWindow ClientManager::createWindow() const { return { {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live" }; }