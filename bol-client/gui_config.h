#pragma once
#include <TGUI/Color.hpp>
#include <TGUI/Layout.hpp>
#include <string>

struct ConfigLabel
{
	std::string label;
	unsigned int text_size = 0;
};

struct ConfigEditBox {
	tgui::Layout2d size;
	unsigned int text_size = 0;
	std::string text;
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

