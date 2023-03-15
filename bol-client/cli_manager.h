#pragma once
#include <TGUI/TGUI.hpp>

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
	void startLoop()
	{
		auto window = createWindow();
		//auto scene = initGuiScene(window);
		mainLoop(window/*, std::move(scene)*/);
	}

private:

	sf::RenderWindow createWindow() const { return { {600, 800}, "Box of Live" }; }

	/*std::unique_ptr<GraphicScene> initGuiScene(sf::RenderWindow& window) const
	{
		std::unique_ptr<GraphicScene> scene = std::make_unique<GraphicScene>(window);
		scene->init();
		return scene;
	}*/

	void mainLoop(sf::RenderWindow& window/*, std::unique_ptr<GraphicScene> scene*/)
	{
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				//scene->handleEvent(event);

				if (event.type == sf::Event::Closed)
					window.close();

				//if (scene->isPressedStartButton())
				//{
				//	std::cout << "start" << std::endl; // формируем запрос на обновление игрового поля и запуск игры
				//}

				//if (scene->isChangedConfig()) // формируем пакет с новым конфигом
				//{
				//	std::cout << "Config changed: " << std::endl;
				//}
			}

			window.clear();
			//scene->drawGui();
			window.display();
		}
	}

};

