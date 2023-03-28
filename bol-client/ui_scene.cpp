#include "pch_client.h"
#include "ui_scene.h"
#include <memory>
#include <game_domain.h>

#include "client_packet.h"
#include "network_client.h"

void GraphicScene::init()
{
	auto fields = tgui::Group::create();

	room_list_ = tgui::ListBox::create();
	room_list_->setSize((int)WIDTH_WINDOW * 0.25, HEIGHT_WINDOW);
	room_list_->setItemHeight(26);
	room_list_->getRenderer()->setBackgroundColor(tgui::Color(210, 210, 210));
	room_list_->setTextAlignment(tgui::ListBox::TextAlignment::Center);
	room_list_->getSharedRenderer()->setBorderColor(tgui::Color::White);
//	room_list_->onItemSelect([] {}); ДОПИСАТЬ ЛОГИКУ
	fields->add(room_list_, "room_list");
	
	auto settings_layout = createLayout({ tgui::Color::White,
		{ tgui::bindRight(room_list_), tgui::bindTop(room_list_) },
		(int)HEIGHT_WINDOW * 0.25, WIDTH_WINDOW - room_list_->getSize().x }
	);

	auto grid = tgui::Grid::create();

	grid->setAutoSize(true);
	grid->setPosition(tgui::bindLeft(room_list_), tgui::bindTop(room_list_));
	settings_layout->add(grid);

	conf_helper_->init(*game_config_);

	conf_helper_->doWithAll([&](const std::string& config_name, ConfigHelper::ConfigRecord& config_record) {
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
	});

	fields->add(settings_layout);

	auto game_layout = createLayout({ tgui::Color::White,
		{ tgui::bindLeft(settings_layout), tgui::bindBottom(settings_layout) },
		HEIGHT_WINDOW - settings_layout->getSize().y, WIDTH_WINDOW - room_list_->getSize().x}
	);
	fields->add(game_layout);
	
	auto canvas = tgui::CanvasSFML::create();
	game_layout->add(canvas, "game_canvas");
	canvas_ = canvas;

	auto buttons = tgui::Group::create();

	auto size_room_list = room_list_->getSize();

	auto button_create_room = createButton({ tgui::Color::Red,
		tgui::Color::Red,
		tgui::Color::Magenta,
		{ tgui::bindLeft(fields),size_room_list.y * 0.9 },
		{ size_room_list.x, size_room_list.y * 0.1 },
		"CREATE ROOM" });
	button_create_room->getRenderer()->setTextColor(tgui::Color::White);

	button_create_room->onPress([] {
		std::cout << "press BCreateR" << std::endl;
		PTCreateRoom packet("room ololo");
		NetworkClient::getInstance().sendPacket(packet);
	});

	auto size_settings_layout = settings_layout->getSize();

	auto button_start = createButton({ tgui::Color::Green,
		tgui::Color::Green,
		tgui::Color::Magenta,
		{size_settings_layout.x,size_settings_layout.y * 0.25 },
		{ size_settings_layout.x *0.25, size_settings_layout.y *0.4 },
		"START" });

	/*button_start->onPress([] {
		std::cout << "press START" << std::endl;
		PTStartGame packet();
		NetworkClient::getInstance().sendPacket(packet);
	});*/


	buttons->add(button_create_room);
	buttons->add(button_start);


	gui_.add(fields, "fields");
	gui_.add(buttons, "buttons");
	

	//field_state_info_->init(game_config_);

	//	timer_.initDouble(0.5);
}

void GraphicScene::backToMenu()
{
	init();
	drawGui();
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


void GraphicScene::createRoom(int id_room, const std::string& room_name)
{
	room_list_->addItem(std::to_string(id_room) +" "+room_name, std::to_string(id_room));
}

void GraphicScene::createRoomList(const std::vector<std::string>& name_room)
{
	auto layout = createLayout({ tgui::Color::Green, { 230,140}, 300, 350 });
	auto grid = tgui::Grid::create();
	grid->setPosition(layout->getPosition());
	grid->setSize(layout->getSize());
	for (auto i = 0; i < name_room.size(); i++) {
		auto label = createLabel({ name_room[i], 16 });
		grid->addWidget(label, i, 0, tgui::Grid::Alignment::Left);
	}
	gui_.add(layout);
	gui_.add(grid);
}

tgui::Panel::Ptr GraphicScene::createLayout(const ConfigLayout& conf) const
{
	auto layout = tgui::Panel::create();
	layout->getRenderer()->setBackgroundColor(conf.color);
	layout->setPosition(conf.position);
	layout->setHeight(conf.height);
	layout->setWidth(conf.width);
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
	auto x_field_size = canvas->getSize().x;
	auto y_field_size = canvas->getSize().y;

	sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black),
			sf::Vertex(sf::Vector2f(x_field_size, 0), sf::Color::Black)
	};

	for (auto i = 0; i < (int)(y_field_size / CELL_SIZE); i++)
	{
		canvas->draw(line, 2, sf::Lines);
		line[0].position.y += CELL_SIZE;
		line[1].position.y += CELL_SIZE;
	}
	line[0].position = sf::Vector2f(0, 0);
	line[1].position = sf::Vector2f(0, y_field_size);
	for (auto i = 0; i < (int)(x_field_size / CELL_SIZE); i++)
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