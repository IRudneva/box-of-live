#pragma once
#include <TGUI/TGUI.hpp>
#include "ui_scene.h"
#include "cli_shared_packet_queue.h"
#include "hv/TcpClient.h"

class ClientManager
{
public:
	void initGameUI(const hv::SocketChannelPtr& channel)
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
					if(graphic_scene_->isPressedChooseRoomButton())
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

	void handleUIEvent(UIEventType event, const hv::SocketChannelPtr& channel) const
	{
		DeserializePacketWriter writer;
		switch (event)
		{
		case UIEventType::PRESSED_BUTTON_CREATE_ROOM:
		{
			std::cout << "press BCreateR" << std::endl;
			std::shared_ptr<PTCreateRoom> pt_create_room = std::make_shared<PTCreateRoom>("room ololo");
			writer.writeDeserializePacket(pt_create_room);
			auto net_pac = writer.getSerializePacket();
			channel->write((uint8_t*)&net_pac->header, (int)sizeof(net_pac->header));
			channel->write((uint8_t*)&net_pac->data, (int)net_pac->data.size());
			break;
		}
		case UIEventType::PRESSED_BUTTON_CHOOSE_ROOM:
		{
			/*std::cout << "press BChooseR" << std::endl;
				
			packet->header = { PacketType::PT_MSG_GET_ROOM_LIST, sizeof(packet->data.data()) };
			packet_queue_->pushPacket(packet);
			packet_queue_->handlePushedPacket();
			auto test = *packet_queue_->popPacket();
			channel->write((uint8_t*)&test.header, (int)sizeof(test.header));
			channel->write((uint8_t*)&test.data, (int)sizeof(test.data.data()));*/
			break;
		}
		case UIEventType::NO_EVENT: break;
		default: ;
		}
	}

	void handlePacket(std::shared_ptr<DeserializePacket> packet, const hv::SocketChannelPtr& channel) const
	{
		std::shared_ptr<DeserializePacket> cur_packet = nullptr;

		packet_queue_->pushPacket(packet);
		packet_queue_->handlePushedPacket();
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

private:
	std::shared_ptr<GraphicScene> graphic_scene_;
	std::shared_ptr<SharedPacketQueue> packet_queue_ = std::make_shared<SharedPacketQueue>();;
	

	sf::RenderWindow createWindow() const { return {  {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live"  }; }

	//void uiLoop(sf::RenderWindow& window) const
	//{
	//	while (window.isOpen())
	//	{
	//		sf::Event event{};
	//		while (window.pollEvent(event))
	//		{
	//			graphic_scene_->handleEvent(event);

	//			if (event.type == sf::Event::Closed)
	//				window.close();

	//			if (auto ui_event = graphic_scene_->checkEvents(); ui_event != UIEventType::NO_EVENT)
	//				handleUIEvent(ui_event);

	//		}

	//		window.clear();
	//		graphic_scene_->drawGui();
	//		window.display();
	//	}
	//}

};

