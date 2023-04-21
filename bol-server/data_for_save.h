#pragma once
#include "game_domain.h"

struct DataForSave
{
	virtual ~DataForSave() = default;
protected:
	DataForSave() = default;
};

struct SaveCellInfo : DataForSave
{
	explicit SaveCellInfo(int id, int ox, int oy, TypeCell type)
	: id_room(id), x(ox), y(oy), cell_type(static_cast<int>(type)) {}

	int id_room;
	int x;
	int y;
	int cell_type;
};

struct SaveBacteriumInfo : DataForSave
{
	explicit SaveBacteriumInfo(int id_r, int ox, int oy, int id, int en, int r, int g, int b)
		:id_room(id_r), pos_x(ox), pos_y(oy), id_type(id), energy(en),red(r), green(g), blue(b){}

	int id_room;
	int pos_x;
	int pos_y;
	int id_type;
	int energy;
	int red;
	int green;
	int blue;
};