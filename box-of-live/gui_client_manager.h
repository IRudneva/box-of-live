#pragma once

#include "domain.h"
#include "graphic_scene.h"

class GuiClientManager
{
public:
	void startLoop()
	{
		auto window = createWindow();
		auto scene = initGuiScene(window);
		mainLoop(window, std::move(scene));
	}

private:

	sf::RenderWindow createWindow() const { return { {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live" }; }

	std::unique_ptr<GraphicScene> initGuiScene(sf::RenderWindow& window) const
	{
		std::unique_ptr<GraphicScene> scene = std::make_unique<GraphicScene>(window);
		scene->init();
		return scene;
	}

	void mainLoop(sf::RenderWindow& window, std::unique_ptr<GraphicScene> scene)
	{
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				scene->handleEvent(event);

				if (event.type == sf::Event::Closed)
					window.close();

				if (scene->isPressedStartButton())
				{
					std::cout << "start" << std::endl; // формируем запрос на обновление игрового поля и запуск игры
				}

				if(scene->isChangedConfig()) // формируем пакет с новым конфигом
				{
					std::cout << "Config changed: " << std::endl;
				}
			}

			window.clear();
			scene->drawGui();
			window.display();
		}
	}
};

