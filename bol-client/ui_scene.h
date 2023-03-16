#pragma once
#include <queue>
#include <TGUI/TGUI.hpp>
#include "gui_config.h"
#include "config_helper.h"
#include "packet_reader.h"

class GraphicScene final
{
public:
	GraphicScene(sf::RenderWindow & window) : gui_(window) {}

	void init();

	void drawGui();

	void update();

	void handleEvent(const sf::Event& event)
	{
		gui_.handleEvent(event);
	}

	bool isPressedCreateRoomButton() const {
		if (button_create_room_->isMouseDown())
			return true;
		return false;
	}

	bool isPressedChooseRoomButton() const {
		if (button_choose_room_->isMouseDown())
			return true;
		return false;
	}

	UIEventType checkEvents()
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

	bool isChangedConfig()
	{
		return conf_helper_.isChanged();
	}

private:
	std::weak_ptr<tgui::CanvasSFML> canvas_;
	tgui::GuiSFML gui_;
	tgui::Button::Ptr button_create_room_;
	tgui::Button::Ptr button_choose_room_;
	std::queue<UIEventType> events_;
//	tgui::Button::Ptr start_button;
	ConfigHelper conf_helper_;
	Timer timer_;
//	std::map<int, tgui::Color> color_bacterium_by_type_;
	//std::shared_ptr<GuiFieldState> field_state_info_ = std::make_shared<GuiFieldState>();
	std::shared_ptr<GameConfig> game_config_ = std::make_shared<GameConfig>();

	void drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canvas) const;
	
	tgui::Panel::Ptr createLayout(const ConfigLayout& conf) const;

	tgui::Button::Ptr createButton(const ConfigButton& conf) const;

	tgui::Label::Ptr createLabel(const ConfigLabel& conf) const;

	tgui::EditBox::Ptr createEditBox(const ConfigEditBox& conf) const;

	//tgui::Color getColorCellByType(TypeCell type, std::optional<BacteriumInfo> inf_bacterium);

	//tgui::Color getCellColorByBacteriumEnergy(int energy, tgui::Color color) const;

	//tgui::Color getCellColorByBacteriumId(int id);

};