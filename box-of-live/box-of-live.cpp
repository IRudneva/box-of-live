#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

class GraphicScene
{
public:
	GraphicScene(sf::RenderWindow & window)
		: gui_(window)
	{}

	void init()
	{
		tgui::Group::Ptr fields = tgui::Group::create();

		tgui::Panel::Ptr base_layer = tgui::Panel::create();
		base_layer->getRenderer()->setBackgroundColor(tgui::Color::Black);

		tgui::Panel::Ptr settings_layout = tgui::Panel::create();
		settings_layout->getRenderer()->setBackgroundColor(tgui::Color::White);
		settings_layout->setPosition(tgui::bindLeft(fields), tgui::bindTop(fields));
		settings_layout->setHeight("200");

		tgui::Panel::Ptr game_layout = tgui::Panel::create();
		game_layout->getRenderer()->setBackgroundColor(tgui::Color::White);
		game_layout->setPosition(tgui::bindLeft(settings_layout), tgui::bindBottom(settings_layout) + 5);
		game_layout->setHeight("395");

		fields->add(base_layer);
		fields->add(settings_layout);
		fields->add(game_layout);

		gui_.add(fields);

		tgui::Group::Ptr buttons = tgui::Group::create();

		tgui::Button::Ptr start_button = tgui::Button::create();
		start_button->getRenderer()->setBackgroundColor(tgui::Color::Yellow);
		start_button->getRenderer()->setBackgroundColorDown(tgui::Color::Blue);
		start_button->getRenderer()->setBackgroundColorHover(tgui::Color::Green);
		start_button->setSize(150, 70);
		start_button->setPosition(tgui::bindLeft(settings_layout) + 600, tgui::bindTop(settings_layout) + 100);
		start_button->setText("START");
		//	start_button->onPress([=] { settings_layout->setVisible(false); });

		buttons->add(start_button);

		gui_.add(buttons);

		tgui::Group::Ptr labels = tgui::Group::create();

		tgui::Label::Ptr label1 = tgui::Label::create();
		label1->setText("Name1:");
		label1->setPosition(tgui::bindLeft(settings_layout) + 50, tgui::bindTop(settings_layout) + 30);
		label1->setTextSize(14);
		labels->add(label1);

		tgui::Label::Ptr label2 = tgui::Label::create();
		label2->setText("Name2:");
		label2->setPosition(tgui::bindLeft(settings_layout) + 50, tgui::bindBottom(label1) + 30);
		label2->setTextSize(14);
		labels->add(label2);

		tgui::Label::Ptr label3 = tgui::Label::create();
		label3->setText("Name3:");
		label3->setPosition(tgui::bindLeft(settings_layout) + 50, tgui::bindBottom(label2) + 30);
		label3->setTextSize(14);
		labels->add(label3);

		gui_.add(labels);

		tgui::Group::Ptr e_box = tgui::Group::create();

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

		gui_.add(e_box);
	}

	void handleEvent(const sf::Event & event)
	{
		gui_.handleEvent(event);
	}

	void update()
	{
		gui_.draw();
		
	/*	sf::RectangleShape rect;
		rect.setFillColor(sf::Color::Black);
		rect.setOutlineColor(sf::Color::Magenta);
		rect.setPosition(40, 40);
		rect.setSize(sf::Vector2f(50, 50));
		if (auto canvas = myCanvas_.lock(); canvas != nullptr) {
			canvas->clear(tgui::Color::Cyan);
			canvas->draw(rect);
			canvas->display();
		}*/
	}
private:
	//std::weak_ptr<tgui::CanvasSFML> myCanvas_;
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
	GraphicScene scene{ window };
	scene.init();
	mainLoop(window, scene);
}