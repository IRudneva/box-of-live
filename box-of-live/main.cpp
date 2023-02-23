#include "graphic_scene.h"


//struct MyGameConfig
//{
//	int enegry_base;
//	int speed;
//};
//
//struct ConfigHelper
//{
//	struct ConfigRecord
//	{
//		int defaultValue;
//		std::function<void(MyGameConfig&, int)> setterFunction;
//	};
//
//	void Init()
//	{
//		ConfigRecord energyBaseRecord;
//		energyBaseRecord.setterFunction = [](MyGameConfig& config, int value) {
//			config.enegry_base = value;
//		};
//		energyBaseRecord.defaultValue = 5;
//		records["enegry_base"] = energyBaseRecord;
//	}
//
//	void SetOption(MyGameConfig & gameConfig, const std::string & str, int value)
//	{
//		records[str].setterFunction(gameConfig, value);
//	}
//
//	void DoWithAll(const std::function<void(const std::string &, int)> & aVisitor)
//	{
//		for (const auto &[optionName, configRecord] : records) {
//			aVisitor(optionName, configRecord.defaultValue);
//		}
//	}
//
//	std::map<std::string, ConfigRecord> records;
//};



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