#include "cli_manager.h"
#include <TGUI/TGUI.hpp>
#include "packet_writer.h"
#include "game_domain.h"
#include <iostream>

void ClientManager::initGameUI(const hv::SocketChannelPtr& channel)
{
	auto window = createWindow();
	graphic_scene_ = std::make_shared<GraphicScene>(window);
	graphic_scene_->init();
	//uiLoop(window);
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
					handleUIEvent(UIEventType::PRESSED_BUTTON_CREATE_ROOM, channel);
				}
				if (graphic_scene_->isPressedChooseRoomButton())
				{
					handleUIEvent(UIEventType::PRESSED_BUTTON_CHOOSE_ROOM, channel);
				}
			}
		}

		window.clear();
		graphic_scene_->drawGui();
		window.display();
	}
}

void ClientManager::handleUIEvent(UIEventType event, const hv::SocketChannelPtr& channel) const
{
	DeserializePacketWriter writer;
	switch (event)
	{
	case UIEventType::PRESSED_BUTTON_CREATE_ROOM:
	{
		std::cout << "press BCreateR" << std::endl;
		std::shared_ptr<PTCreateRoom> pt_create_room = std::make_shared<PTCreateRoom>("room ololo");
		{/*
			std::string test;
			char t = '*';
			for (auto i = 0; i < 1000000; i++)
			{
				test.push_back(t);
			}
			std::shared_ptr<PTCreateRoom> pt_create_room = std::make_shared<PTCreateRoom>(test);
			writer.writeDeserializePacket(pt_create_room);
			auto net_pac = writer.getSerializePacket();
			channel->write((uint8_t*)&net_pac->header, (int)sizeof(net_pac->header));
			channel->write(net_pac->data.data(), (int)net_pac->data.size());*/
		}
		writer.writeDeserializePacket(pt_create_room);
		auto net_pac = writer.getSerializePacket();
		channel->write((uint8_t*)&net_pac->header, (int)sizeof(net_pac->header));
		channel->write(net_pac->data.data(), (int)net_pac->data.size());
		break;
	}
	case UIEventType::PRESSED_BUTTON_CHOOSE_ROOM:
	{
		std::cout << "press BChooseR" << std::endl;
		std::shared_ptr<PTGetRoomList> pt_ger_room_list = std::make_shared<PTGetRoomList>();
		writer.writeDeserializePacket(pt_ger_room_list);
		auto net_pac = writer.getSerializePacket();
		channel->write((uint8_t*)&net_pac->header, (int)sizeof(net_pac->header));
		channel->write(net_pac->data.data(), (int)net_pac->data.size());
		break;
	}
	case UIEventType::NO_EVENT: break;
	default:;
	}
}

void ClientManager::handlePacket(std::shared_ptr<DeserializePacket> packet, const hv::SocketChannelPtr& channel) const
{
	std::shared_ptr<DeserializePacket> cur_packet = nullptr;

	packet_queue_->pushPacket(packet);
	cur_packet = packet_queue_->popPacket();

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