#pragma once
#include "game_domain.h"

struct DbBacteriumColorState
{
	DbBacteriumColorState() = default;
	explicit DbBacteriumColorState(int id_r, int id, int r, int g, int b)
		:id_room(id_r), id_type(id), red(r), green(g), blue(b){}

	int id_room;
	int id_type;
	int red;
	int green;
	int blue;
};

struct pairhash {
public:
	std::size_t operator()(const std::pair<int, int>&x) const
	{
		size_t h_x = hasher_(x.first);
		size_t h_y = hasher_(x.second);
		return h_x * 137 + h_y * (137 * 137);

	}
private:
	std::hash<int> hasher_;
};

struct DbCellState
{
	TypeCell cell_type;
	bool is_super_grass;
	int bact_type;
	int energy;
};

struct DbSaveRoomState
{
	int id_room;
	bool is_deleted = false;


	using XYPos = std::pair<int, int>;
	std::unordered_map<XYPos, DbCellState, pairhash> cell_states;

	void formDataCells(const std::vector<DeletedPosition>& del_inf, const std::vector<GrassInfo>& grass_inf, const std::vector<BacteriumInfo>& bact_inf)
	{
		for (const auto& pos : del_inf)
		{
			DbCellState state = { TypeCell::EMPTY };
			XYPos key = { pos.x, pos.y };
			cell_states.insert({ key, state });
		}

		for (const auto& grass : grass_inf)
		{
			DbCellState state = { TypeCell::GRASS, grass.is_super_grass, 0, 0 };
			XYPos key = { grass.x, grass.y };
			cell_states.insert({ key, state });
		}

		for (const auto& bact : bact_inf)
		{
			DbCellState state = { TypeCell::BACTERIUM, false,bact.id_type, bact.energy };
			XYPos key = { bact.x, bact.y };
			cell_states.insert({ key, state });
		}
	}
};
