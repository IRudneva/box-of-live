#pragma once

#include "domain.h"
#include "graphic_scene.h"

class GuiClientManager
{
public:
	void startLoop()
	{
		sf::RenderWindow window{ {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live" };
		GraphicScene scene(window);
		scene.init();
		mainLoop(window, scene);
	}

private:
	void mainLoop(sf::RenderWindow& window, GraphicScene& scene)
	{
		while (window.isOpen())
		{
			sf::Event event{};
			while (window.pollEvent(event))
			{
				scene.handleEvent(event);

				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();
			scene.update();
			window.display();
		}
	}
};

