#pragma once
#include "packet_domain.h"
#include "gui_config.h"
#include <queue>

#include "config_helper.h"

class GraphicScene final
{
public:
	GraphicScene(sf::RenderWindow& window) : gui_(window) {}

	void init();

	void initConnectionFlag(bool status);

	void initConfigGrid(uint32_t id_room, bool status);

	void initGameLayout(uint32_t id_room);

	void initButtonStart(bool status) const { gui_.get("button_start")->setEnabled(status); }

	void initButtonCloseRoom(bool status) const { gui_.get("button_close_room")->setEnabled(status); }

	void backToMenu(uint32_t id_room);

	void backToMenu();

	void drawGui(uint32_t id_room, const std::vector<GrassInfo>& cell_info, const std::vector<BacteriumInfo>& bact_inf);

	void update();

	void handleEvent(const sf::Event& event);

	void createRoom(int id_room, const std::string& room);

	void createRoomList(const std::vector<Room>& room_list);

private:
	using IdRoom = int;
	std::map<IdRoom, std::weak_ptr<tgui::CanvasSFML>> canvas_for_room_;
	tgui::GuiSFML gui_;
	tgui::ListBox::Ptr room_list_;
	tgui::Panel::Ptr connection_flag_;
	std::map<IdRoom, tgui::Grid::Ptr> config_grid_for_room_;
	int id_selected_room_ = 0;
	//tgui::Grid::Ptr config_grid_;
	std::unique_ptr<ConfigHelper> conf_helper_ = std::make_unique<ConfigHelper>();
//	Timer timer_;
	std::map<int, tgui::Color> color_bacterium_by_type_;
	//std::shared_ptr<GuiFieldState> field_state_info_ = std::make_shared<GuiFieldState>();
	//std::shared_ptr<GameConfig> game_config_ = std::make_shared<GameConfig>();

	void drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canvas) const;
	
	tgui::Panel::Ptr createLayout(const ConfigLayout& conf) const;

	tgui::Button::Ptr createButton(const ConfigButton& conf) const;

	tgui::Label::Ptr createLabel(const ConfigLabel& conf) const;

	tgui::EditBox::Ptr createEditBox(const ConfigEditBox& conf) const;

	tgui::Color getCellColorByBacteriumEnergy(int energy, tgui::Color color) const;

	tgui::Color getCellColorByBacteriumId(int id);
};
