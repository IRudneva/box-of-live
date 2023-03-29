#pragma once
#include "gui_config.h"
#include <queue>

#include "config_helper.h"

class GraphicScene final
{
public:
	GraphicScene(sf::RenderWindow& window) : gui_(window) {}

	void init();

	void initConnectionFlag(bool status);

	void backToMenu();

	void drawGui();

	void update();

	void handleEvent(const sf::Event& event);

	void createRoom(int id_room, const std::string& room_name);

	void createRoomList(const std::vector<std::string>& name_room);

//	bool isChangedConfig() { return conf_helper_.isChanged(); }

private:
	std::weak_ptr<tgui::CanvasSFML> canvas_;
	tgui::GuiSFML gui_;
	tgui::ListBox::Ptr room_list_;
	tgui::Panel::Ptr connection_flag_;
	std::unique_ptr<ConfigHelper> conf_helper_ = std::make_unique<ConfigHelper>();
//	Timer timer_;
//	std::map<int, tgui::Color> color_bacterium_by_type_;
	//std::shared_ptr<GuiFieldState> field_state_info_ = std::make_shared<GuiFieldState>();
	std::unique_ptr<GameConfig> game_config_ = std::make_unique<GameConfig>();

	void drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canvas) const;
	
	tgui::Panel::Ptr createLayout(const ConfigLayout& conf) const;

	tgui::Button::Ptr createButton(const ConfigButton& conf) const;

	tgui::Label::Ptr createLabel(const ConfigLabel& conf) const;

	tgui::EditBox::Ptr createEditBox(const ConfigEditBox& conf) const;

	//tgui::Color getColorCellByType(TypeCell type, std::optional<BacteriumInfo> inf_bacterium);

	//tgui::Color getCellColorByBacteriumEnergy(int energy, tgui::Color color) const;

	//tgui::Color getCellColorByBacteriumId(int id);
};
