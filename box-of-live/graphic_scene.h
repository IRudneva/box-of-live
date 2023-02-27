#pragma once
#include "bacterium.h"
#include "field_state.h"

struct ConfigLabel
{
	std::string label;
	unsigned int text_size;
	tgui::Layout2d position;
};

struct ConfigButton
{
	tgui::Color color_background;
	tgui::Color color_down;
	tgui::Color color_hover;
	tgui::Layout2d position;
	tgui::Layout2d size;
	std::string text;
};

struct ConfigLayout
{
	tgui::Color color;
	tgui::Layout2d position;
	tgui::Layout height;
};

class GraphicScene final
{
public:
	GraphicScene(sf::RenderWindow & window)
		: gui_(window)
	{}

	void init();

	void handleEvent(const sf::Event & event) { gui_.handleEvent(event); }

	void update();

	tgui::Button::Ptr start() { return start_button; }

private:
	std::weak_ptr<tgui::CanvasSFML> canvas_;
	tgui::GuiSFML gui_;
	std::unique_ptr<FieldState> game_state_ = std::make_unique<FieldState>();
	std::map<unsigned int, tgui::Color> color_bacterium_by_type_;
	tgui::Button::Ptr start_button;

	tgui::Panel::Ptr createLayout(const ConfigLayout& conf) const;

	tgui::Button::Ptr createButton(const ConfigButton& conf) const;

	tgui::Label::Ptr createLabel(const ConfigLabel& conf) const;

	void drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canv) const;

	tgui::Color getCellColorByBacteriumId(unsigned int id);

	tgui::Color getColorCellByType(std::shared_ptr<Cell> cell);

	Timer timer_ = {0.5};
};