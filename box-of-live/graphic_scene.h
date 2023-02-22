#pragma once
#include "bacterium.h"
#include "cell_factory.h"

struct ConfigLabel
{
	std::string label;
	tgui::Layout2d position;
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
	tgui::Panel::Ptr createLayout(const tgui::Color& color, const tgui::Layout2d& pos, const tgui::Layout& height);

	tgui::Button::Ptr createButton(const tgui::Color& col, const tgui::Color& col_down, const tgui::Color& col_hover, const tgui::Layout2d& pos, const tgui::Layout2d& size, const std::string& text);

	tgui::Label::Ptr createLabel(const std::string& text, unsigned int text_size, const tgui::Layout2d& pos);


	std::weak_ptr<tgui::CanvasSFML> canvas_;
	tgui::GuiSFML gui_;
	std::vector<sf::RectangleShape> cells_;
	std::unique_ptr<FieldState> field_state_ = std::make_unique<FieldState>();
	tgui::Button::Ptr start_button;
};