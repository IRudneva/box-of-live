#pragma once
//#include <TGUI/TGUI.hpp>
//#include <SFML/Graphics.hpp>
#include "ui_event.h"
#include "gui_config.h"
#include <queue>

class GraphicScene final
{
public:
	GraphicScene(sf::RenderWindow& window) : gui_(window) {}

	void init();

	void drawGui();

	void update();

	void handleEvent(const sf::Event& event);

	bool isPressedCreateRoomButton() const;

	bool isPressedChooseRoomButton() const;

	UIEventType checkEvents();

//	bool isChangedConfig() { return conf_helper_.isChanged(); }

private:
	std::weak_ptr<tgui::CanvasSFML> canvas_;
	tgui::GuiSFML gui_;
	tgui::Button::Ptr button_create_room_;
	tgui::Button::Ptr button_choose_room_;
	std::queue<UIEventType> events_;
//	tgui::Button::Ptr start_button;
//	ConfigHelper conf_helper_;
//	Timer timer_;
//	std::map<int, tgui::Color> color_bacterium_by_type_;
	//std::shared_ptr<GuiFieldState> field_state_info_ = std::make_shared<GuiFieldState>();
//	std::shared_ptr<GameConfig> game_config_ = std::make_shared<GameConfig>();

	void drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canvas) const;
	
	tgui::Panel::Ptr createLayout(const ConfigLayout& conf) const;

	tgui::Button::Ptr createButton(const ConfigButton& conf) const;

	tgui::Label::Ptr createLabel(const ConfigLabel& conf) const;

	tgui::EditBox::Ptr createEditBox(const ConfigEditBox& conf) const;

	//tgui::Color getColorCellByType(TypeCell type, std::optional<BacteriumInfo> inf_bacterium);

	//tgui::Color getCellColorByBacteriumEnergy(int energy, tgui::Color color) const;

	//tgui::Color getCellColorByBacteriumId(int id);

};