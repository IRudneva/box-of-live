#pragma once
#include "packet_domain.h"
#include "gui_config.h"
#include <queue>

#include "config_helper.h"

class GraphicScene final
{
public:
	GraphicScene(sf::RenderWindow& window) : gui_(window) {}

	void initGraphicScene();

	void initGameLayout(uint32_t id_room);

	void initConnectionFlag(bool status);

	void initConfigGrid(uint32_t id_room, bool status);

	void initButtonStart(bool status) const { gui_.get("button_start")->setEnabled(status); }

	void initButtonCloseRoom(bool status) const { gui_.get("button_close_room")->setEnabled(status); }

	void setConfigForRoom(uint32_t id_room, std::shared_ptr<GameConfig> conf);

	void clearRoomList() const;

	void clearGameCanvas() const;

	void drawGameCanvas(uint32_t id_room, const std::vector<GrassInfo>& cell_info, const std::vector<BacteriumInfo>& bact_inf);

	void update();

	void handleEvent(const sf::Event& event);

	void createRoom(int id_room, const std::string& room);

	void createRoomList(const std::vector<Room>& room_list);

private:
	using IdRoom = int;

	std::weak_ptr<tgui::CanvasSFML> game_canvas_;
	tgui::GuiSFML gui_;
	tgui::ListBox::Ptr room_list_;
	tgui::Panel::Ptr connection_flag_;

	struct UIConfig
	{
		UIConfig() = default;
		UIConfig(tgui::Grid::Ptr g, std::shared_ptr<GameConfig> gc, std::unique_ptr<ConfigHelper> ch)
		: grid(g), config(std::move(gc)), helper(std::move(ch)){}

		tgui::Grid::Ptr grid;
		std::shared_ptr<GameConfig> config = std::make_shared<GameConfig>();
		std::unique_ptr<ConfigHelper> helper = std::make_unique<ConfigHelper>();
	};

	std::map<IdRoom, UIConfig> config_for_room_;

	int id_selected_room_ = 0;
	std::map<int, tgui::Color> color_bacterium_by_type_;

	void drawMarkupField(std::shared_ptr<tgui::CanvasSFML> canvas) const;
	
	tgui::Panel::Ptr createLayout(const ConfigLayout& conf) const;

	tgui::Button::Ptr createButton(const ConfigButton& conf) const;

	tgui::Label::Ptr createLabel(const ConfigLabel& conf) const;

	tgui::EditBox::Ptr createEditBox(const ConfigEditBox& conf) const;

	tgui::Color getCellColorByBacteriumEnergy(int id_room, int energy, tgui::Color color) const;

	tgui::Color getCellColorByBacteriumId(int id);
};
