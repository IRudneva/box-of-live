#pragma once
#include <mutex>
#include <TGUI/TGUI.hpp>
#include "ui_scene.h"
#include "shared_packet_queue.h"
#include "hv/TcpClient.h"

//enum class MsgType : uint32_t
//{
//	MSG_GET_ROOM_LIST
//};
//
//struct Msg
//{
//	MsgType type = MsgType::MSG_GET_ROOM_LIST;
//
//	template<class T>
//	void pack(T& packer) {
//		packer(static_cast<uint32_t>(type));
//	}
//
//};
//
//struct IdRoom
//{
//	static int generateId() {
//		++id;
//		return id;
//	}
//	static void reset() { id = 0; }
//private:
//	IdRoom() = default;
//	inline static int id = 0;
//};
//
//struct GameRoom
//{
//	GameRoom() { id_room = IdRoom::generateId(); }
//	int field_state = 1;
//	int getIdRoom() const { return id_room; }
//	template<class T>
//	void pack(T& packer) {
//		packer(id_room, field_state);
//	}
//
//private:
//	int id_room = 0;
//};
//
//
//enum class PacketType : uint32_t
//{
//	PT_ROOM_LIST
//};
//
//struct Packet
//{
//	PacketType type;
//	std::map<int, GameRoom> room_list;
//
//	template<class T>
//	void pack(T& packer) {
//		auto t = static_cast<uint32_t>(type);
//		packer(t, room_list);
//	}
//
//};

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
		PacketWriter writer;
		std::shared_ptr<Packet> packet = std::make_shared<Packet>();
		switch (event)
		{
		case UIEventType::PRESSED_BUTTON_CREATE_ROOM:
		{
			std::cout << "press BCreateR" << std::endl;
			packet = std::make_shared<Packet>();
			packet->header = { PacketType::PT_MSG_CREATE_ROOM, 0 };
			packet->data = {};
			packet_queue_->pushPacket(packet);
			packet_queue_->handlePushedPacket();
			auto p1 = packet_queue_->popPacket();
			//	writer.writePacket(packet_queue_->popPacket());
			channel->write(&p1, sizeof(p1->header)/*writer.getData().data(), writer.getData().size()*/);
			break;
		}
		case UIEventType::PRESSED_BUTTON_CHOOSE_ROOM:
		{
			std::cout << "press BChooseR" << std::endl;
			packet = std::make_shared<Packet>();
			packet->header = { PacketType::PT_MSG_GET_ROOM_LIST, 0 };
			packet->data = {};
			packet_queue_->pushPacket(packet);
			packet_queue_->handlePushedPacket();
			auto p2 = packet_queue_->popPacket();
			channel->write(&p2, sizeof(p2->header));
			break;
		}
		case UIEventType::NO_EVENT: break;
		default: ;
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

