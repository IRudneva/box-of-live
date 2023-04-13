#pragma once
#include <iostream>

#include "packet_domain.h"
#include "gui_config.h"
#include <queue>

#include "config_helper.h"

class GraphicScene final
{
public:
	explicit GraphicScene(sf::RenderWindow& window) : gui_(window) {}

	void initGraphicScene();

	void update() { gui_.draw(); }

	void drawGameCanvas(uint32_t id_room, const std::vector<GrassInfo>& cell_info, const std::vector<BacteriumInfo>& bact_inf);

	void handleEvent(const sf::Event& event) { gui_.handleEvent(event); }

	void createRoom(int id_room, const std::string& room, const GameConfig& conf) const { room_list_->addItem(std::to_string(id_room) + " " + room, std::to_string(id_room)); }

	void createRoomList(const std::vector<Room>& room_list);

	void onNetworkDisconnect() const;

	void onNetworkConnect() const { initConnectionFlag(true); }

	void onChooseRoom(const GameConfig& conf);

	void onCloseRoom(int id_room) const;

	void setConfig(const GameConfig& conf) { config_.config = conf; }

private:
	using IdRoom = int;

	std::weak_ptr<tgui::CanvasSFML> game_canvas_;
	tgui::GuiSFML gui_;
	tgui::ListBox::Ptr room_list_;
	tgui::Panel::Ptr connection_flag_;

	struct UIConfig
	{
		UIConfig() = default;
		tgui::Grid::Ptr grid = tgui::Grid::create();
		GameConfig config;
		std::unique_ptr<ConfigHelper> helper = std::make_unique<ConfigHelper>();
	} config_;

	int id_selected_room_ = 0;
	std::map<int, tgui::Color> color_bacterium_by_type_;

	void initLayout();

	void initCanvas();

	void initButtons();

	void initGrid();

	void initConnectionFlag(bool status) const;

	void initButtonStart(bool status) const { gui_.get("button_start")->setEnabled(status); }

	void initButtonCloseRoom(bool status) const { gui_.get("button_close_room")->setEnabled(status); }

	void initConfigGrid(bool status) const;

	void deleteRoom(uint32_t id_room) const;

	void clearRoomList() const { room_list_->removeAllItems(); }

	void clearGameCanvas() const;

	void drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canvas) const;

	tgui::Panel::Ptr createLayout(const ConfigLayout& conf) const;

	tgui::Button::Ptr createButton(const ConfigButton& conf) const;

	tgui::Label::Ptr createLabel(const ConfigLabel& conf) const;

	tgui::EditBox::Ptr createEditBox(const ConfigEditBox& conf) const;

	void createListBox(const ConfigListBox& conf);

	tgui::Color getCellColorByBacteriumEnergy(int energy, tgui::Color color) const;

	tgui::Color getCellColorByBacteriumId(int id);
};
