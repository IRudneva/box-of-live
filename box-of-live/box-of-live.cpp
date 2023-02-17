#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

struct GraphicConfig
{
	struct Settings {
		tgui::Color color_layout = tgui::Color::White;
		tgui::Layout height_layout = "200";

		std::string text_start_button = "START";
		tgui::Color color_start_button_background = tgui::Color::Green;
		tgui::Color color_start_button_background_down = tgui::Color::Red;
		tgui::Color color_start_button_background_hover = tgui::Color::Magenta;
		tgui::Layout2d size_start_button = { 150,70 };
		tgui::Layout2d pos_start_button = { 500, 55 };

		int count_labels = 3;
		std::vector<std::string> text_labels = {"Name_1", "Name_2", "Name_3"};
		std::vector<tgui::Layout2d> pos_labels = { {50, 30},{50, 60}, {50, 90} };
		size_t text_size = 14;
	} settings;

	struct GameField {
		tgui::Color color_layout = tgui::Color::White;
		tgui::Layout height_layout = "400";
	} game_field;
};

class GraphicScene
{
public:
	GraphicScene(sf::RenderWindow & window)
		: gui_(window)
	{}

	void init(const GraphicConfig& config)
	{
		auto fields = tgui::Group::create();

		auto settings_layout = tgui::Panel::create();
		settings_layout->getRenderer()->setBackgroundColor(config.settings.color_layout);
		settings_layout->setPosition(tgui::bindLeft(fields), tgui::bindTop(fields));
		settings_layout->setHeight(config.settings.height_layout);

		auto game_layout = tgui::Panel::create();
		game_layout->getRenderer()->setBackgroundColor(config.game_field.color_layout);
		game_layout->setPosition(tgui::bindLeft(settings_layout), tgui::bindBottom(settings_layout) );
		game_layout->setHeight(config.game_field.height_layout);

		auto canvas = tgui::CanvasSFML::create();
		game_layout->add(canvas, "my_canvas");
		canvas_ = canvas;

		fields->add(settings_layout);
		fields->add(game_layout);

		gui_.add(fields);

		auto buttons = tgui::Group::create();

		auto start_button = tgui::Button::create();
		start_button->getRenderer()->setBackgroundColor(config.settings.color_start_button_background);
		start_button->getRenderer()->setBackgroundColorDown(config.settings.color_start_button_background_down);
		start_button->getRenderer()->setBackgroundColorHover(config.settings.color_start_button_background_hover);
		start_button->setSize(config.settings.size_start_button);
		start_button->setPosition(config.settings.pos_start_button);
		start_button->setText(config.settings.text_start_button);
		//	start_button->onPress([=] { settings_layout->setVisible(false); });

		buttons->add(start_button);

		gui_.add(buttons);

		tgui::Group::Ptr labels = tgui::Group::create();
	
		for(int i = 0; i < config.settings.count_labels; i++)
		{
			auto label = tgui::Label::create();
			label->setText(config.settings.text_labels[i]);
			label->setTextSize(config.settings.text_size);
			label->setPosition(config.settings.pos_labels[i]);
			labels->add(label);
		}
		gui_.add(labels);

		/*tgui::Group::Ptr e_box = tgui::Group::create();

		auto editBox1 = tgui::EditBox::create();
		editBox1->setSize(100, 16);
		editBox1->setTextSize(14);
		editBox1->setPosition(tgui::bindRight(label1) + 20, tgui::bindTop(label1) );
		editBox1->setDefaultText("Value1");

		e_box->add(editBox1);

		auto editBox2 = tgui::EditBox::create();
		editBox2->setSize(100, 16);
		editBox2->setTextSize(14);
		editBox2->setPosition(tgui::bindRight(label2) + 20, tgui::bindTop(label2));
		editBox2->setDefaultText("Value2");

		e_box->add(editBox2);

		auto editBox3 = tgui::EditBox::create();
		editBox3->setSize(100, 16);
		editBox3->setTextSize(14);
		editBox3->setPosition(tgui::bindRight(label3) + 20, tgui::bindTop(label3));
		editBox3->setDefaultText("Value3");

		e_box->add(editBox3);

		gui_.add(e_box);*/
	}

	void handleEvent(const sf::Event & event)
	{
		gui_.handleEvent(event);
	}

	void update()
	{
		gui_.draw();
		
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color::White);
		rect.setOutlineThickness(1.0f);
		rect.setOutlineColor(sf::Color::Black);
		rect.setPosition(0, 0);
		rect.setSize(sf::Vector2f(10, 10));
		
		std::vector<sf::RectangleShape> rects1(80, rect);
		std::vector<sf::RectangleShape> rects2(40, rect);

		sf::Vector2f pos(0,0);

		if (auto canvas = canvas_.lock(); canvas != nullptr) {
			canvas->clear(tgui::Color::Green);

			for (auto i = 0; i < rects2.size(); i++)
			{
				rects2[i].setPosition(pos.x, pos.y);
				canvas->draw(rects2[i]);
				for (auto j = 0; j < rects1.size(); j++)
				{
					rects1[j].setPosition(pos.x+10, pos.y);
					pos = rects1[j].getPosition();
					canvas->draw(rects1[j]);
				}
				
				pos = sf::Vector2f(rects2[i].getPosition().x, rects2[i].getPosition().y+10);
			}
			canvas->display();
		}
	}
private:
	std::weak_ptr<tgui::CanvasSFML> canvas_;
	tgui::GuiSFML gui_;
};

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
	sf::RenderWindow window{ {800, 600}, "Box of Live" };
	GraphicScene scene(window);
	GraphicConfig gc;
	scene.init(gc);
	mainLoop(window, scene);
}