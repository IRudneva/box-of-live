#include "graphic_scene.h"
#include <memory>

void GraphicScene::init()
{
	auto fields = tgui::Group::create();

	auto settings_layout = createLayout(tgui::Color::White, { tgui::bindLeft(fields), tgui::bindTop(fields) }, "200");

	auto game_layout = createLayout(tgui::Color::White, { tgui::bindLeft(settings_layout), tgui::bindBottom(settings_layout) }, "400");

	auto canvas = tgui::CanvasSFML::create();
	game_layout->add(canvas, "my_canvas");
	canvas_ = canvas;

	fields->add(settings_layout);
	fields->add(game_layout);

	gui_.add(fields);

	auto buttons = tgui::Group::create();

	start_button = createButton(tgui::Color::Green, tgui::Color::Red, tgui::Color::Magenta, 
		{ 150, 70 }, { 500, 55 }, "START");

	start_button->onPress([=] { 
		cf.addGrass(30);
		cf.addBacterium(1, 30);
	});

	buttons->add(start_button);

	gui_.add(buttons);

	tgui::Group::Ptr labels = tgui::Group::create();
	std::vector<ConfigLabel> labels_conf = { {"Number of bacterial species:", {50, 30}},{"Bacteria energy:", {50, 60}},{"Speed:", {50, 90}} };

	for (int i = 0; i < labels_conf.size(); i++)
	{
		auto label = createLabel(labels_conf[i].label, 14, labels_conf[i].position);
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


void GraphicScene::update()
{
	gui_.draw();

	sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black),
			sf::Vertex(sf::Vector2f(800, 0), sf::Color::Black)
	};

	if (auto canvas = canvas_.lock(); canvas != nullptr) {
		canvas->clear(tgui::Color::White);
		for (auto i = 0; i < 50; i++)
		{
			canvas->draw(line, 2, sf::Lines);
			line[0].position.y += 8;
			line[1].position.y += 8;
		}
		line[0].position = sf::Vector2f(0, 0);
		line[1].position = sf::Vector2f(0, 400);

		for (auto i = 0; i < 100; i++)
		{
			canvas->draw(line, 2, sf::Lines);
			line[0].position.x += 8;
			line[1].position.x += 8;
		}

		cf.update();

		auto m = cf.getData();
		for (auto&[p, c] : m)
		{
			sf::RectangleShape cell;
			cell.setSize(sf::Vector2f(c->getSize().x.getValue(), c->getSize().y.getValue()));
			cell.setPosition(p.x, p.y);
			cell.setFillColor(c->getColor());
			cell.setOutlineColor(sf::Color::Black);
			canvas->draw(cell);
		}

		canvas->display();
	}
}

tgui::Panel::Ptr GraphicScene::createLayout(const tgui::Color& color, const tgui::Layout2d& pos, const tgui::Layout& height)
{
	auto layout = tgui::Panel::create();
	layout->getRenderer()->setBackgroundColor(color);
	layout->setPosition(pos);
	layout->setHeight(height);
	return layout;
}

tgui::Button::Ptr GraphicScene::createButton(const tgui::Color& col, const tgui::Color& col_down, const tgui::Color& col_hover, const tgui::Layout2d& size, const tgui::Layout2d& pos, const std::string& text)
{
	auto button = tgui::Button::create();
	button->getRenderer()->setBackgroundColor(col);
	button->getRenderer()->setBackgroundColorDown(col_down);
	button->getRenderer()->setBackgroundColorHover(col_hover);
	button->setSize(size);
	button->setPosition(pos);
	button->setText(text);
	return button;
}

tgui::Label::Ptr GraphicScene::createLabel(const std::string& text, unsigned int text_size, const tgui::Layout2d& pos)
{
	auto label = tgui::Label::create();
	label->setText(text);
	label->setTextSize(text_size);
	label->setPosition(pos);
	return label;
}
