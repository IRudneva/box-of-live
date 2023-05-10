#pragma once
#include "packet_domain.h"
#include "gui_config.h"
#include <queue>

#include "config_helper.h"

class GraphicScene final
{
public:
	explicit GraphicScene(sf::RenderWindow& window) : gui_(window) {}

	void initGraphicScene();

	void update();

	void setGameCanvasSize(int delta) const;

	void clearCurrentFieldState() { current_field_state_.reset(); }

	void updateCurrentFieldState(const std::vector<GrassInfo>& grass_info, const std::vector<BacteriumInfo>& bact_inf, const std::vector<DeletedPosition>& deleted_pos);

	void handleEvent(const sf::Event& event) { gui_.handleEvent(event); }

	void createRoom(int id_room, const std::string& room, const GameConfig& conf) const { room_list_->addItem(std::to_string(id_room) + " " + room, std::to_string(id_room)); }

	void createRoomList(const std::vector<Room>& room_list);

	void onNetworkDisconnect();

	void onNetworkConnect() const { initConnectionFlag(true); }

	void onChooseRoom(const std::vector<GrassInfo>& grass_info, const std::vector<BacteriumInfo>& bact_inf, const GameConfig& conf, const std::map<int, SrvColor>& color, bool status);

	void onCloseRoom(int id_room);

	void setConfig(const GameConfig& conf) { config_.config = conf; }

	tgui::ChatBox::Ptr getLogBox()const { return log_box_; }

	void setColorForBacterium(const std::map<int, SrvColor>& color_map);

	void initButtonStart(bool is_active) const;

	void initConfigGrid(bool status) const;

private:
	using IdRoom = int;

	std::weak_ptr<tgui::CanvasSFML> game_canvas_;
	tgui::GuiSFML gui_;
	tgui::ListBox::Ptr room_list_;
	tgui::Panel::Ptr connection_flag_;
	tgui::ChatBox::Ptr log_box_;

	struct UIConfig
	{
		UIConfig() = default;
		tgui::Grid::Ptr grid = tgui::Grid::create();
		GameConfig config;
		std::unique_ptr<ConfigHelper> helper = std::make_unique<ConfigHelper>();
		bool is_init = false;
	} config_;

	int id_selected_room_ = -1;
	std::map<int, tgui::Color> color_bacterium_by_type_;

	struct UIPosition
	{
		explicit UIPosition(int ox, int oy) : x(static_cast<float>(ox*CELL_SIZE)), y(static_cast<float>(oy*CELL_SIZE)){}

		bool operator==(const UIPosition& other) const { return x == other.x && y == other.y; }

		float x = 0;
		float y = 0;
	};

	struct UIPositionHasher
	{
		size_t operator() (const UIPosition& p) const
		{
			size_t h_x = ui_hasher_(p.x);
			size_t h_y = ui_hasher_(p.y);
			return h_x * 137 + h_y * (137 * 137);
		}
	private:
		std::hash<int> ui_hasher_;
	};

	struct DeltaState
	{
		std::unordered_map<UIPosition, bool, UIPositionHasher> grass_info;
		std::unordered_map<UIPosition, BacteriumInfo, UIPositionHasher> bact_inf;

		void reset() 
		{
			grass_info.clear();
			bact_inf.clear();
		}
	} current_field_state_;

	void initLayout();

	void initCanvas();

	void initButtons();

	void initGrid();

	void initConnectionFlag(bool status) const;

	void initButtonCloseRoom(bool status) const { gui_.get("button_close_room")->setEnabled(status); }

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
};
