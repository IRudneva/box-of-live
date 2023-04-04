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

	room_list_->onItemSelect([this](const tgui::String& item_name, const tgui::String& id)
	{
		uint32_t id_room = static_cast<uint32_t>(std::stoi(id.toStdString()));
		client_packet::PTChooseRoom packet(id_room);
		NetworkClient::getInstance().sendPacket(packet);
		id_selected_room_ = static_cast<int>(id_room);
	});

	fields->add(room_list_, "room_list");
	
	auto settings_layout = createLayout({ tgui::Color::White,
		{ tgui::bindRight(room_list_), tgui::bindTop(room_list_) },
		(int)HEIGHT_WINDOW * 0.25, WIDTH_WINDOW - room_list_->getSize().x }
	);

	
	connection_flag_ = createLayout({ tgui::Color::Red,
		{ settings_layout->getSize().x - (int)settings_layout->getSize().y * 0.1 , settings_layout->getPosition().y },
		(int)settings_layout->getSize().y * 0.1, (int)settings_layout->getSize().y * 0.1 });


	settings_layout->add(connection_flag_);

	fields->add(settings_layout, "settings_layout");

	auto buttons = tgui::Group::create();

	auto size_room_list = room_list_->getSize();

	auto button_create_room = createButton({ tgui::Color::Yellow,
		tgui::Color::Yellow,
		tgui::Color::Magenta,
		{ tgui::bindLeft(fields),size_room_list.y * 0.9 },
		{ size_room_list.x, size_room_list.y * 0.1 },
		"CREATE ROOM" });
	//button_create_room->getRenderer()->setTextColor(tgui::Color::White);

	button_create_room->onPress([] {
		client_packet::PTCreateRoom packet;
		NetworkClient::getInstance().sendPacket(packet);
	});

	auto size_settings_layout = settings_layout->getSize();

	auto button_start = createButton({ tgui::Color::Green,
		tgui::Color::Green,
		tgui::Color::Magenta,
		{size_settings_layout.x,size_settings_layout.y * 0.15 },
		{ size_settings_layout.x *0.25, size_settings_layout.y *0.4 },
		"START" });

	button_start->setEnabled(false);

	button_start->onPress([this] {
		std::cout << "press START" << std::endl;
		client_packet::PTStartGame packet(static_cast<uint32_t>(id_selected_room_));
		NetworkClient::getInstance().sendPacket(packet);
	});

	auto button_close_room = createButton({ tgui::Color::Red,
		tgui::Color::Red,
		tgui::Color::Magenta,
		{tgui::bindLeft(button_start),tgui::bindBottom(button_start) },
		{ button_start->getSize().x, button_start->getSize().y },
		"CLOSE ROOM" });

	button_close_room->getRenderer()->setTextColor(tgui::Color::White);
	button_close_room->setEnabled(false);

	button_close_room->onPress([this] {
		client_packet::PTCloseRoom packet(static_cast<uint32_t>(id_selected_room_));
		NetworkClient::getInstance().sendPacket(packet);
	});

	buttons->add(button_create_room, "button_create_room");
	buttons->add(button_start, "button_start");
	buttons->add(button_close_room, "button_close_room");


	gui_.add(fields, "fields");
	gui_.add(buttons, "buttons");
	

	//field_state_info_->init(game_config_);

	//	timer_.initDouble(0.5);
}

void GraphicScene::backToMenu(uint32_t id_room)
{
	if (auto canv = canvas_for_room_[static_cast<int>(id_room)].lock(); canv != nullptr) {
		canv->clear(tgui::Color::White);
		canv->display();
	}
}

void GraphicScene::backToMenu()
{
	for(const auto& [id, canvas] : canvas_for_room_)
	{
		backToMenu(id);
	}
}

void GraphicScene::initGameLayout(uint32_t id_room)
{
	auto settings_pos = gui_.get("settings_layout")->getPosition();
	auto settings_size = gui_.get("settings_layout")->getSize();

	auto game_layout = createLayout({ tgui::Color::White,
		{ settings_pos.x, settings_pos.y+settings_size.y},
		HEIGHT_WINDOW - settings_size.y,
		WIDTH_WINDOW - room_list_->getSize().x }
	);

	auto canvas = tgui::CanvasSFML::create();
	game_layout->add(canvas, "game_canvas_" + std::to_string(id_room));

	gui_.add(game_layout, "game_layout_" + std::to_string(id_room));

	canvas_for_room_[static_cast<int>(id_room)] = canvas;
}

void GraphicScene::drawGui(uint32_t id_room, const std::vector<GrassInfo>& grass_info, const std::vector<BacteriumInfo>& bact_inf)
{
	if (auto canv = canvas_for_room_[static_cast<int>(id_room)].lock(); canv != nullptr) {
		canv->clear(tgui::Color::White);
		sf::RectangleShape cell_shape;
		cell_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
		for (const auto& grass : grass_info)
		{
			float pos_x = static_cast<float>(grass.x *CELL_SIZE);
			float pos_y = static_cast<float>(grass.y * CELL_SIZE);
			cell_shape.setPosition(pos_x, pos_y); // 
			cell_shape.setFillColor(tgui::Color::Green);
			cell_shape.setOutlineColor(sf::Color::Black);
			canv->draw(cell_shape);
		}
		for (const auto& bact : bact_inf)
		{
			float pos_x = static_cast<float>(bact.x *CELL_SIZE);
			float pos_y = static_cast<float>(bact.y * CELL_SIZE);
			cell_shape.setPosition(pos_x, pos_y); // 
			cell_shape.setFillColor(getCellColorByBacteriumEnergy(bact.energy, getCellColorByBacteriumId(bact.id_type)));
			cell_shape.setOutlineColor(sf::Color::Black);
			canv->draw(cell_shape);
		}
		drawMarkupField(canv);
		canv->display();
	}
}

void GraphicScene::update()
{
	gui_.draw();
}

void GraphicScene::handleEvent(const sf::Event& event)
{
	gui_.handleEvent(event);
}

void GraphicScene::initConfigGrid(uint32_t id_room, bool status)
{
	std::for_each(config_grid_for_room_.begin(), config_grid_for_room_.end(),
		[](auto config_grid)
	{
		config_grid.second->setVisible(false);
		config_grid.second->setEnabled(false);
	}
	);
	config_grid_for_room_.at(static_cast<int>(id_room))->setVisible(status);
	config_grid_for_room_.at(static_cast<int>(id_room))->setEnabled(status);
	std::cout << "set visible config room " << id_room << std::endl;
}

void  GraphicScene::initConnectionFlag(bool status)
{
	connection_flag_->getRenderer()->setBackgroundColor((status) ? tgui::Color::Green : tgui::Color::Red);
	if(connection_flag_)
	{
		client_packet::PTGetRoomList get_room_list;
		NetworkClient::getInstance().sendPacket(get_room_list);
	}
}

void GraphicScene::createRoom(int id_room, const std::string& room)
{
	auto id_item = room_list_->addItem(std::to_string(id_room) +" "+room, std::to_string(id_room));

	std::shared_ptr<GameConfig> game_config = std::make_shared<GameConfig>();
	conf_helper_->init(*game_config);
	client_packet::PTChangeConfig pt_def_config({ static_cast<uint32_t>(id_room), game_config });
	NetworkClient::getInstance().sendPacket(pt_def_config);

	auto grid = tgui::Grid::create();
	grid->setAutoSize(true);
	grid->setPosition(tgui::bindRight(room_list_), tgui::bindTop(room_list_));
	//
	conf_helper_->doWithAll([&](const std::string& config_name, ConfigHelper::ConfigRecord& config_record) {
		auto label = createLabel({ config_name, 14 });

		grid->addWidget(label, config_record.row_id, config_record.column_id * 2, tgui::Grid::Alignment::Left);

		auto edit_box = createEditBox({ {100, 16}, 14, std::to_string(config_record.default_value) });

		edit_box->onTextChange([this, edit_box, &config_record]() {
			auto text = edit_box->getText();
			if (!text.empty())
			{
				config_record.setter_function(text.toInt());
				/*
				std::shared_ptr<GameConfig> gc = std::make_shared<GameConfig>();
				for(const auto& [name, conf] : conf_helper_->getRecords())
				{
					
				}
				client_packet::PTChangeConfig pt_change_config({ static_cast<uint32_t>(id_selected_room_), gc});
				NetworkClient::getInstance().sendPacket(pt_change_config);*/
			}
		});

		grid->addWidget(edit_box,
			config_record.row_id,
			config_record.column_id * 2 + 1,
			tgui::Grid::Alignment::Left);
	});
	grid->setVisible(false);
	grid->setEnabled(false);
	gui_.add(grid, "grid_" + std::to_string(id_room));
	config_grid_for_room_.insert({id_room, grid});
	initGameLayout(id_room);
	
}

void GraphicScene::createRoomList(const std::vector<Room>& room_list)
{
	for (const auto& room: room_list) {
		createRoom(static_cast<int>(room.id), room.name);
	}
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
//
//tgui::Color GraphicScene::getColorCellByType(TypeCell type, std::shared_ptr<BacteriumInfo> inf_bacterium)
//{
//	switch (type)
//	{
//	case TypeCell::GRASS:
//		return tgui::Color::Green;
//	case TypeCell::BACTERIUM:
//	{
//		if (inf_bacterium!=nullptr)
//		{
//			tgui::Color color_bacterium = getCellColorByBacteriumId(inf_bacterium->id_type);
//			return getCellColorByBacteriumEnergy(inf_bacterium->energy, color_bacterium);
//		}
//	}
//	case TypeCell::EMPTY:
//		return tgui::Color::White;
//	}
//	return tgui::Color::White;
//}

tgui::Color GraphicScene::getCellColorByBacteriumEnergy(int energy, tgui::Color color) const
{
	/*if (energy < game_config_->energy_base * 0.25)
		return { color.getRed(), color.getGreen(), color.getBlue(), static_cast<uint8_t>(color.getAlpha() * 0.4) };
	if (energy < game_config_->energy_base * 0.7)
		return { color.getRed(), color.getGreen(), color.getBlue(), static_cast<uint8_t>(color.getAlpha() * 0.8) };*/
	return color;
}

tgui::Color GraphicScene::getCellColorByBacteriumId(int id)
{
	if (color_bacterium_by_type_.find(id) != color_bacterium_by_type_.end())
		return color_bacterium_by_type_.at(id);

	auto red = getRandomInt(0, 255);
	auto blue = getRandomInt(0, 255);
	auto green = getRandomInt(0, 255);

	color_bacterium_by_type_.insert({ id,tgui::Color(red, green, blue) });
	return color_bacterium_by_type_.at(id);
}