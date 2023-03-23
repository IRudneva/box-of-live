#include "pch_client.h"
#include "ui_scene.h"
#include <memory>
#include <game_domain.h>

void GraphicScene::init()
{
	auto fields = tgui::Group::create();
	auto welcome_layout = createLayout({ tgui::Color::White, { tgui::bindLeft(fields), tgui::bindTop(fields) }, HEIGHT_WINDOW });
	fields->add(welcome_layout);
	//auto settings_layout = createLayout({ tgui::Color::White, { tgui::bindLeft(fields), tgui::bindTop(fields) }, "200" });
	//auto game_layout = createLayout({ tgui::Color::White, { tgui::bindLeft(settings_layout), tgui::bindBottom(settings_layout) }, "400" });
	//auto canvas = tgui::CanvasSFML::create();
	//game_layout->add(canvas, "game_canvas");
	//canvas_ = canvas;
	//fields->add(settings_layout);
	//fields->add(game_layout);

	auto buttons = tgui::Group::create();

	button_create_room_ = createButton({ tgui::Color::Green, tgui::Color::Green, tgui::Color::Magenta,
		{ 350, 150 }, { 150, 70 }, "CREATE ROOM" });
	

	button_choose_room_ = createButton({ tgui::Color::Yellow, tgui::Color::Yellow, tgui::Color::Magenta,
	{ 350, 250 }, { 150, 70 }, "CHOOSE A ROOM" });

	buttons->add(button_create_room_);
	buttons->add(button_choose_room_);
	/*start_button = createButton({ tgui::Color::Green, tgui::Color::Red, tgui::Color::Magenta,
		{ 500, 55 }, { 150, 70 }, "START" });

	buttons->add(start_button);

	auto grid = tgui::Grid::create();

	grid->setAutoSize(true);
	settings_layout->add(grid);

	conf_helper_.init(*game_config_); 

	conf_helper_.doWithAll([&](const std::string& config_name, ConfigHelper::ConfigRecord& config_record) {
		auto label = createLabel({ config_name, 14 });
		grid->addWidget(label, config_record.row_id, config_record.column_id * 2, tgui::Grid::Alignment::Left);

		auto edit_box = createEditBox({ {100, 16}, 14, std::to_string(config_record.default_value) });
		edit_box->onTextChange([edit_box, &config_record]() {
			auto text = edit_box->getText();
			if (!text.empty()) {
				config_record.setter_function(text.toInt());
			}
		});
		grid->addWidget(edit_box,
			config_record.row_id,
			config_record.column_id * 2 + 1,
			tgui::Grid::Alignment::Left);
	});*/

	gui_.add(fields);
	gui_.add(buttons);
	//field_state_info_->init(game_config_);

//	timer_.initDouble(0.5);
}

void GraphicScene::drawGui()
{
	gui_.draw();
	if (auto canvas = canvas_.lock(); canvas != nullptr) {
		canvas->clear(tgui::Color::White);
		drawMarkupField(canvas);
		canvas->display();
	}
}

void GraphicScene::update()
{
	/*gui_.draw();

	if (auto canvas = canvas_.lock(); canvas != nullptr) {
		canvas->clear(tgui::Color::White);
*/
		//if (start_button->isMouseDown())
			//field_state_info_->reset();

		//if (timer_.timedOut())
			//field_state_info_->update();
		
		//auto field_data = field_state_info_->getCellInfo();

		//for (const auto& cell : field_data)
		//{
		//	sf::RectangleShape cell_shape;
		//	cell_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE)); //
		//	float pos_x = static_cast<float>(cell.x *CELL_SIZE);
		//	float pos_y = static_cast<float>(cell.y * CELL_SIZE);
		//	cell_shape.setPosition(pos_x, pos_y); // 
		//	cell_shape.setFillColor(getColorCellByType(cell.type, cell.bacterium_info));
		//	cell_shape.setOutlineColor(sf::Color::Black);
		//	canvas->draw(cell_shape);
		//}
	//	drawMarkupField(canvas);

	//	canvas->display();
	//}
}

void GraphicScene::handleEvent(const sf::Event& event)
{
	gui_.handleEvent(event);
}

bool GraphicScene::isPressedCreateRoomButton() const {
	if (button_create_room_->isMouseDown())
		return true;
	return false;
}

bool GraphicScene::isPressedChooseRoomButton() const {
	if (button_choose_room_->isMouseDown())
		return true;
	return false;
}

UIEventType GraphicScene::checkEvents()
{
	if (isPressedCreateRoomButton())
		events_.push(UIEventType::PRESSED_BUTTON_CREATE_ROOM);
	if (isPressedChooseRoomButton())
		events_.push(UIEventType::PRESSED_BUTTON_CHOOSE_ROOM);
	if (events_.empty())
		return UIEventType::NO_EVENT;
	auto top_event = events_.front();
	events_.pop();
	return top_event;
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
	return label;
}

void GraphicScene::drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canvas) const
{
	sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black),
			sf::Vertex(sf::Vector2f(WIDTH_PLAYING_FIELD, 0), sf::Color::Black)
	};

	for (auto i = 0; i < HEIGHT_PLAYING_FIELD / CELL_SIZE; i++)
	{
		canvas->draw(line, 2, sf::Lines);
		line[0].position.y += CELL_SIZE;
		line[1].position.y += CELL_SIZE;
	}
	line[0].position = sf::Vector2f(0, 0);
	line[1].position = sf::Vector2f(0, HEIGHT_PLAYING_FIELD);
	for (auto i = 0; i < WIDTH_PLAYING_FIELD / CELL_SIZE; i++)
	{
		canvas->draw(line, 2, sf::Lines);
		line[0].position.x += CELL_SIZE;
		line[1].position.x += CELL_SIZE;
	}
}

tgui::EditBox::Ptr GraphicScene::createEditBox(const ConfigEditBox& conf) const
{
	auto edit_box = tgui::EditBox::create();
	edit_box->setInputValidator(tgui::EditBox::Validator::Int);
	edit_box->setSize(conf.size);
	edit_box->setTextSize(conf.text_size);
	edit_box->setDefaultText(conf.text);
	return edit_box;
}

//tgui::Color GraphicScene::getColorCellByType(TypeCell type, std::optional<BacteriumInfo> inf_bacterium)
//{
//	switch (type)
//	{
//	case TypeCell::GRASS:
//		return tgui::Color::Green;
//	case TypeCell::BACTERIUM:
//	{
//		if (inf_bacterium.has_value())
//		{
//			tgui::Color color_bacterium = getCellColorByBacteriumId(inf_bacterium.value().id_type);
//			return getCellColorByBacteriumEnergy(inf_bacterium.value().energy, color_bacterium);
//		}
//	}
//	case TypeCell::EMPTY:
//		return tgui::Color::White;
//	}
//	return tgui::Color::White;
//}
//
//tgui::Color GraphicScene::getCellColorByBacteriumEnergy(int energy, tgui::Color color) const
//{
//	if (energy < game_config_->energy_base * 0.25)
//		return { color.getRed(), color.getGreen(), color.getBlue(), static_cast<uint8_t>(color.getAlpha() * 0.4) };
//	if (energy < game_config_->energy_base * 0.7)
//		return { color.getRed(), color.getGreen(), color.getBlue(), static_cast<uint8_t>(color.getAlpha() * 0.8) };
//	return color;
//}
//
//tgui::Color GraphicScene::getCellColorByBacteriumId(int id)
//{
//	if (color_bacterium_by_type_.find(id) != color_bacterium_by_type_.end())
//		return color_bacterium_by_type_.at(id);
//
//	auto red = getRandomInt(0, 255);
//	auto blue = getRandomInt(0, 255);
//	auto green = getRandomInt(0, 255);
//
//	color_bacterium_by_type_.insert({ id,tgui::Color(red, green, blue) });
//	return color_bacterium_by_type_.at(id);
//}