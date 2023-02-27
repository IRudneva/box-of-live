#include "graphic_scene.h"


void mainLoop(sf::RenderWindow& window, GraphicScene& scene)
{
	while (window.isOpen())
	{
		sf::Event event;
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

int main()
{
	sf::RenderWindow window{ {WIDTH_WINDOW, HEIGHT_WINDOW}, "Box of Live" };
	GraphicScene scene(window);
	scene.init();
	mainLoop(window, scene);
}