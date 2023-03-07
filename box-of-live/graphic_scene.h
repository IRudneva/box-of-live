#pragma once
#include "bacterium.h"
#include "field_state.h"
#include "gui_config.h"

class GraphicScene final
{
public:
	GraphicScene(sf::RenderWindow & window) : gui_(window) {}

	void init();

	void update();

	void handleEvent(const sf::Event& event) { gui_.handleEvent(event); }

private:
	std::weak_ptr<tgui::CanvasSFML> canvas_;
	tgui::GuiSFML gui_;
	std::unique_ptr<FieldState> game_state_ = std::make_unique<FieldState>();
	std::map<int, tgui::Color> color_bacterium_by_type_;
	tgui::Button::Ptr start_button;
	ConfigHelper conf_helper_;
	Timer timer_;

	void drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canvas) const;
	
	tgui::Panel::Ptr createLayout(const ConfigLayout& conf) const;

	tgui::Button::Ptr createButton(const ConfigButton& conf) const;

	tgui::Label::Ptr createLabel(const ConfigLabel& conf) const;

	tgui::EditBox::Ptr createEditBox(const ConfigEditBox& conf) const;

	tgui::Color getCellColorByBacteriumEnergy(int energy, tgui::Color color) const;

	tgui::Color getColorCellByType(std::shared_ptr<Cell> cell);

	tgui::Color getCellColorByBacteriumId(int id);
};