#include "pch_client.h"
#include "graphic_scene.h"
#include <memory>
#include <game_domain.h>
#include <iostream>
#include <utility>

#include "client_packet.h"
#include "network_client.h"
#include "client_logger.h"
#include "log_duration.h"

void GraphicScene::initGraphicScene()
{
	initLayout();
	initCanvas();
	initGrid();
	initButtons();
}

void GraphicScene::initLayout()
{
	auto fields = tgui::Group::create();

	const auto common_layout = createLayout({ tgui::Color::Blue,
		{ 0, 0 },
		HEIGHT_WINDOW, WIDTH_WINDOW
		});

	createListBox({ {(int)common_layout->getSize().x * 0.25, (int)common_layout->getSize().y * 0.8},
		26,
		tgui::Color(210, 210, 210),
		tgui::ListBox::TextAlignment::Center,
		tgui::Color::White
		});

	auto settings_layout = createLayout({ tgui::Color::White,
		{ tgui::bindRight(room_list_), tgui::bindTop(room_list_) },
		(int)HEIGHT_WINDOW * 0.25, WIDTH_WINDOW - room_list_->getSize().x }
	);

	connection_flag_ = createLayout({ tgui::Color::Red,
		{ settings_layout->getSize().x - (int)settings_layout->getSize().y * 0.1 , settings_layout->getPosition().y },
		(int)settings_layout->getSize().y * 0.1, (int)settings_layout->getSize().y * 0.1 });


	settings_layout->add(connection_flag_);

	auto game_layout = createLayout({ tgui::Color::White,
		{ settings_layout->getPosition().x, settings_layout->getPosition().y + settings_layout->getSize().y},
		/*HEIGHT_WINDOW - settings_layout->getSize().y*/HEIGHT_PLAYING_FIELD,
		/*WIDTH_WINDOW - room_list_->getSize().x*/WIDTH_PLAYING_FIELD }
	);

	common_layout->add(room_list_, "room_list");
	common_layout->add(settings_layout, "settings_layout");
	common_layout->add(game_layout, "game_layout");

	log_box_ = tgui::ChatBox::create();
	log_box_->setSize(WIDTH_WINDOW, 200);
	log_box_->setTextSize(11);
	log_box_->getRenderer()->setBackgroundColor(tgui::Color::Black);
	log_box_->setPosition(tgui::bindLeft(common_layout), tgui::bindBottom(common_layout));
	log_box_->setLinesStartFromTop();
	fields->add(common_layout, "common_layout");

	gui_.add(fields, "fields");
	gui_.add(log_box_, "log_box");

	ClientLogger::getInstance()->init(log_box_);
}

void GraphicScene::initCanvas()
{
	auto canvas = tgui::CanvasSFML::create();

	const auto game_layout = gui_.get("game_layout")->cast<tgui::Panel>();

	game_layout->add(canvas, "game_canvas");

	game_canvas_ = canvas;

	clearGameCanvas();
}

void GraphicScene::initButtons()
{
	auto buttons = tgui::Group::create();

	const auto common_layout_size = gui_.get("common_layout")->getSize();

	auto button_close_room = createButton({ tgui::Color::Red,
		tgui::Color::Red,
		tgui::Color::Magenta,
		{tgui::bindLeft(room_list_),tgui::bindBottom(room_list_) },
		{ room_list_->getSize().x, common_layout_size.y * 0.1 },
		"CLOSE ROOM" });

	button_close_room->getRenderer()->setTextColor(tgui::Color::White);
	button_close_room->setEnabled(false);

	button_close_room->onPress([this] {
		client_packet::PTCloseRoom packet(static_cast<uint32_t>(id_selected_room_));
		NetworkClient::getInstance()->sendPacket(packet);
	});

	const auto button_create_room = createButton({ tgui::Color::Yellow,
		tgui::Color::Yellow,
		tgui::Color::Magenta,
		{ tgui::bindLeft(button_close_room), tgui::bindBottom(button_close_room) },
		{ button_close_room->getSize().x, button_close_room->getSize().y},
		"CREATE ROOM" });

	button_create_room->onPress([this] {
		config_.helper->init(config_.config);
		const client_packet::PTCreateRoom packet(std::make_shared<GameConfig>(config_.config));
		NetworkClient::getInstance()->sendPacket(packet);
	});

	auto size_settings_layout = gui_.get("settings_layout")->getSize();

	auto button_start = createButton({ tgui::Color::Green,
		tgui::Color::Green,
		tgui::Color::Magenta,
		{size_settings_layout.x,size_settings_layout.y * 0.15 },
		{ size_settings_layout.x *0.25, size_settings_layout.y *0.4 },
		"START" });

	button_start->setEnabled(false);

	button_start->onPress([this] {
		client_packet::PTStartGame packet(static_cast<uint32_t>(id_selected_room_), std::make_shared<GameConfig>(config_.config));
		NetworkClient::getInstance()->sendPacket(packet); //////////////////////////////////// ТУТ ОТПРАВЛЯЕТСЯ КОНФИГ, РАЗМЕР ПОЛЯ ПРОКИНУТЬ В NServer
		clearGameCanvas();
		clearCurrentFieldState();
	});

	buttons->add(button_create_room, "button_create_room");
	buttons->add(button_start, "button_start");
	buttons->add(button_close_room, "button_close_room");

	gui_.add(buttons, "buttons");
}

void GraphicScene::initGrid()
{
	config_.grid->setAutoSize(true);
	
	config_.grid->setPosition(tgui::bindRight(room_list_), tgui::bindTop(room_list_));

	config_.helper->init(config_.config);

	config_.helper->doWithAll([this](const std::string& config_name, ConfigHelper::ConfigRecord& config_record) {
		auto label = createLabel({ config_name, 13 });

		config_.grid->addWidget(label, config_record.row_id, config_record.column_id * 2, tgui::Grid::Alignment::Left);

		auto edit_box = createEditBox({ {50, 15}, 13, std::to_string(config_record.default_value) });

		edit_box->onTextChange([this, edit_box, &config_record]() {
			auto text = edit_box->getText();
			if (!text.empty())
			{
				config_record.setter_function(text.toInt());
			}
		});

		config_.grid->addWidget(edit_box,
			config_record.row_id,
			config_record.column_id * 2 + 1,
			tgui::Grid::Alignment::Left);
	});

	auto label = createLabel({ "game_field_size", 13 });
	config_.grid->addWidget(label, 7, 2, tgui::Grid::Alignment::Left);
	auto tabs = tgui::Tabs::create();
	tabs->setTabHeight(15);
	tabs->add("small");
	tabs->add("middle");
	tabs->add("large");

	tabs->onTabSelect([this, tabs]
	{
		auto indx = tabs->getSelected();
		auto game_field_size = gui_.get("game_layout")->getSize();
		if (indx == "small")
		{
			config_.config.delta_game_field_size = 5;
			setGameCanvasSize(config_.config.delta_game_field_size);
		}
		else if (indx == "middle")
		{
			config_.config.delta_game_field_size = 8;
			setGameCanvasSize(config_.config.delta_game_field_size);
		}
		else if (indx == "large")
		{
			config_.config.delta_game_field_size = 10;
			setGameCanvasSize(config_.config.delta_game_field_size);
		}

	});
	config_.grid->addWidget(tabs, 7, 3, tgui::Grid::Alignment::Left);

	config_.grid->setVisible(false);
	config_.grid->setEnabled(false);

	gui_.add(config_.grid, "grid");
}

void GraphicScene::onNetworkDisconnect()
{
	clearGameCanvas();
	clearCurrentFieldState();
	initConnectionFlag(false);
	initButtonStart(false);
	initButtonCloseRoom(false);
	clearRoomList();
}

void GraphicScene::onChooseRoom(const std::vector<GrassInfo>& grass_info, const std::vector<BacteriumInfo>& bact_inf, const GameConfig& conf, const std::map<int, SrvColor>& color)
{
	initButtonStart(true);
	initButtonCloseRoom(true);
	initConfigGrid(true);
	setConfig(conf);
	setGameCanvasSize(conf.delta_game_field_size);
	setColorForBacterium(color);
	clearCurrentFieldState();

	std::vector<DeletedPosition> empty_vect;
	updateCurrentFieldState(grass_info, bact_inf, empty_vect);
}

void GraphicScene::onCloseRoom(int id_room)
{
	deleteRoom(id_room);
	clearGameCanvas();
	clearCurrentFieldState();
	initButtonStart(false);
	initButtonCloseRoom(false);
}

void GraphicScene::initConfigGrid(bool status) const
{
	config_.grid->setEnabled(status);
	config_.grid->setVisible(status);
}

void GraphicScene::clearGameCanvas() const
{
	if (auto canv = game_canvas_.lock(); canv != nullptr) {
		canv->clear(tgui::Color::White);
		canv->display();
	}
}
//
//void GraphicScene::saveLastDeltaForRoom(int id_room, const std::vector<GrassInfo>& cell_info, const std::vector<BacteriumInfo>& bact_inf)
//{
//	last_state_for_room_[static_cast<int>(id_room)].bact_inf = bact_inf;
//	last_state_for_room_[static_cast<int>(id_room)].grass_info = cell_info;
//}

//void GraphicScene::drawGameCanvas(uint32_t id_room, const std::vector<GrassInfo>& grass_info, const std::vector<BacteriumInfo>& bact_inf/*, const std::vector<DeletedPosition>& deleted_positions*/)
//{
//	//LOG_DURATION("draw");
//
//	if (auto canv = game_canvas_.lock(); canv != nullptr) {
//		canv->clear(tgui::Color::White);
//		sf::RectangleShape cell_shape;
//		cell_shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
//		for (const auto& grass : grass_info)
//		{
//			const float pos_x = static_cast<float>(grass.x *CELL_SIZE);
//			const float pos_y = static_cast<float>(grass.y * CELL_SIZE);
//			cell_shape.setPosition(pos_x, pos_y); 
//			cell_shape.setFillColor(tgui::Color::Green);
//			cell_shape.setOutlineColor(sf::Color::Black);
//			canv->draw(cell_shape);
//		}
//		for (const auto& bact : bact_inf)
//		{
//			const float pos_x = static_cast<float>(bact.x *CELL_SIZE);
//			const float pos_y = static_cast<float>(bact.y * CELL_SIZE);
//			cell_shape.setPosition(pos_x, pos_y);
//			cell_shape.setFillColor(getCellColorByBacteriumEnergy(bact.energy, getCellColorByBacteriumId(bact.id_type)));
//			cell_shape.setOutlineColor(sf::Color::Black);
//			canv->draw(cell_shape);
//		}
//		/*for(const auto& pos:deleted_positions)
//		{
//			const float pos_x = static_cast<float>(pos.x *CELL_SIZE);
//			const float pos_y = static_cast<float>(pos.y * CELL_SIZE);
//			cell_shape.setPosition(pos_x, pos_y);
//			cell_shape.setFillColor(tgui::Color::White);
//			cell_shape.setOutlineColor(sf::Color::White);
//			canv->draw(cell_shape);
//		}*/
//		drawMarkupField(canv);
//		canv->display();
//	}
//}

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

void  GraphicScene::initConnectionFlag(bool status) const
{
	connection_flag_->getRenderer()->setBackgroundColor(status ? tgui::Color::Green : tgui::Color::Red);
	if(connection_flag_)
	{
		const client_packet::PTGetRoomList get_room_list;
		NetworkClient::getInstance()->sendPacket(get_room_list);
	}
}

void GraphicScene::deleteRoom(uint32_t id_room) const
{
	room_list_->removeItemById(std::to_string(id_room));
}

void GraphicScene::createRoomList(const std::vector<Room>& room_list)
{
	for (const auto& room : room_list) {
		createRoom(static_cast<int>(room.id), room.name, *room.config);
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

tgui::EditBox::Ptr GraphicScene::createEditBox(const ConfigEditBox& conf) const
{
	auto edit_box = tgui::EditBox::create();
	edit_box->setInputValidator(tgui::EditBox::Validator::Int);
	edit_box->setSize(conf.size);
	edit_box->setTextSize(conf.text_size);
	edit_box->setDefaultText(conf.text);
	return edit_box;
}

void GraphicScene::createListBox(const ConfigListBox& conf)
{
	room_list_ = tgui::ListBox::create();
	room_list_->setSize(conf.size);
	room_list_->setItemHeight(conf.item_height);
	room_list_->getRenderer()->setBackgroundColor(conf.color_background);
	room_list_->setTextAlignment(conf.al);
	room_list_->getSharedRenderer()->setBorderColor(conf.color_border);

	room_list_->onItemSelect([this](const tgui::String& item_name, const tgui::String& id)
	{
		if (item_name.empty() || id.empty())
			return;

		const uint32_t id_room = static_cast<uint32_t>(std::stoi(id.toStdString()));
		id_selected_room_ = static_cast<int>(id_room);

		client_packet::PTChooseRoom packet(id_selected_room_);
		NetworkClient::getInstance()->sendPacket(packet);
	});
}

tgui::Color GraphicScene::getCellColorByBacteriumEnergy(int energy, tgui::Color color) const
{
	if (energy < config_.config.energy_base * 0.25)
		return { color.getRed(), color.getGreen(), color.getBlue(), static_cast<uint8_t>(color.getAlpha() * 0.4) };
	if (energy < config_.config.energy_base * 0.7)
		return { color.getRed(), color.getGreen(), color.getBlue(), static_cast<uint8_t>(color.getAlpha() * 0.8) };
	return color;
}
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
