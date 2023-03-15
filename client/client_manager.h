#pragma once
#include <TGUI/TGUI.hpp>
#include "msgpack.hpp"

enum class MsgType
{
	MSG_GET_ROOM_LIST
};

struct Msg
{
	MsgType type;
	
};


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
				//	std::cout << "start" << std::endl; // ��������� ������ �� ���������� �������� ���� � ������ ����
				//}

				//if (scene->isChangedConfig()) // ��������� ����� � ����� ��������
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

