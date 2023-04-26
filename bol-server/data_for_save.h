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
	template <typename T, typename U>
	std::size_t operator()(const std::pair<T, U> &x) const
	{
		return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
	}
};

struct DbCellState
{
	TypeCell cell_type;
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
			DbCellState state = { TypeCell::GRASS, 0, 0 };
			XYPos key = { grass.x, grass.y };
			cell_states.insert({ key, state });
		}

		for (const auto& bact : bact_inf)
		{
			DbCellState state = { TypeCell::BACTERIUM, bact.id_type, bact.energy };
			XYPos key = { bact.x, bact.y };
			cell_states.insert({ key, state });
		}
	}
};
