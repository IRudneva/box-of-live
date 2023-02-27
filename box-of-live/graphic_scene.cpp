#include "graphic_scene.h"
#include <memory>

void GraphicScene::init()
{
	auto fields = tgui::Group::create();
	auto settings_layout = createLayout({ tgui::Color::White, { tgui::bindLeft(fields), tgui::bindTop(fields) }, "200" });
	auto game_layout = createLayout({ tgui::Color::White, { tgui::bindLeft(settings_layout), tgui::bindBottom(settings_layout) }, "400" });
	auto canvas = tgui::CanvasSFML::create();
	game_layout->add(canvas, "game_canvas");
	canvas_ = canvas;
	fields->add(settings_layout);
	fields->add(game_layout);

	auto buttons = tgui::Group::create();
	start_button = createButton({ tgui::Color::Green, tgui::Color::Red, tgui::Color::Magenta,
		{ 500, 55 }, { 150, 70 }, "START" });
	start_button->onPress([=] { 
		game_state_->addGrass(200);
		game_state_->addColonyBacterium(20);
	});
	buttons->add(start_button);

	tgui::Group::Ptr labels = tgui::Group::create();
	std::vector<ConfigLabel> labels_conf = {
		{"Number of bacterial species:",14, {50, 30}},
		{"Bacteria energy:",14, {50, 60}},
		{"Speed:",14, {50, 90}}
	};
	for (auto& l : labels_conf)
	{
		auto label = createLabel(l);
		labels->add(label);
	}
	
	gui_.add(fields);
	gui_.add(buttons);
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

	if (auto canvas = canvas_.lock(); canvas != nullptr) {
		canvas->clear(tgui::Color::White);

		if (timer_.timedOut()) {
			game_state_->update();
		}

		auto field_data = game_state_->getData();
		
		for (const auto&[pos, cell] : field_data)
		{
			sf::RectangleShape cell_shape;
			cell_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE)); //
			float pos_x = pos.x *CELL_SIZE;
			float pos_y = pos.y * CELL_SIZE;
			cell_shape.setPosition( pos_x,  pos_y); // 
			cell_shape.setFillColor(getColorCellByType(cell));
			cell_shape.setOutlineColor(sf::Color::Black);
			canvas->draw(cell_shape);
		}
		
		drawMarkupField(canvas);

		canvas->display();
	}
}

tgui::Panel::Ptr GraphicScene::createLayout(const ConfigLayout& conf) const
{
	auto layout = tgui::Panel::create();
	layout->getRenderer()->setBackgroundColor(conf.color);
	layout->setPosition(conf.position);
	layout->setHeight(conf.height);
	return layout;
}

tgui::Button::Ptr GraphicScene::createButton(const ConfigButton& conf) const
{
	auto button = tgui::Button::create();
	button->getRenderer()->setBackgroundColor(conf.color_background);
	button->getRenderer()->setBackgroundColorDown(conf.color_down);
	button->getRenderer()->setBackgroundColorHover(conf.color_hover);
	button->setSize(conf.size);
	button->setPosition(conf.position);
	button->setText(conf.text);
	return button;
}

tgui::Label::Ptr GraphicScene::createLabel(const ConfigLabel& conf) const
{
	auto label = tgui::Label::create();
	label->setText(conf.label);
	label->setTextSize(conf.text_size);
	label->setPosition(conf.position);
	return label;
}

void GraphicScene::drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canv) const
{
	sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black),
			sf::Vertex(sf::Vector2f(WIDTH_PLAYING_FIELD, 0), sf::Color::Black)
	};

	for (auto i = 0; i < HEIGHT_PLAYING_FIELD / CELL_SIZE; i++)
	{
		canv->draw(line, 2, sf::Lines);
		line[0].position.y += CELL_SIZE;
		line[1].position.y += CELL_SIZE;
	}
	line[0].position = sf::Vector2f(0, 0);
	line[1].position = sf::Vector2f(0, HEIGHT_PLAYING_FIELD);
	for (auto i = 0; i < WIDTH_PLAYING_FIELD / CELL_SIZE; i++)
	{
		canv->draw(line, 2, sf::Lines);
		line[0].position.x += CELL_SIZE;
		line[1].position.x += CELL_SIZE;
	}
}

tgui::Color GraphicScene::getColorCellByType(std::shared_ptr<Cell> cell)
{
	switch (auto type = cell->getCellType(); type)
	{
	case TypeCell::GRASS:
		return tgui::Color::Green;
	case TypeCell::BACTERIUM:
	{
		Cell& a = *cell;
		auto id = dynamic_cast<Bacterium&>(a).getIdType();
		return getCellColorByBacteriumId(id);
	}
	case TypeCell::EMPTY:
		return tgui::Color::White;
	}
}

tgui::Color GraphicScene::getCellColorByBacteriumId(unsigned int id)
{
	if (color_bacterium_by_type_.find(id) != color_bacterium_by_type_.end())
	{
		return color_bacterium_by_type_.at(id);
	}

	auto red = getRandomUInt(0, 255);
	auto blue = getRandomUInt(0, 255);
	auto green = getRandomUInt(0, 255);
	color_bacterium_by_type_[id] = tgui::Color(red, green, blue);
	return color_bacterium_by_type_.at(id);
}