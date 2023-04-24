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
	explicit SaveCellInfo(int id, int ox, int oy, TypeCell type, int bact_id_type, int ener)
	: id_room(id), x(ox), y(oy), cell_type(static_cast<int>(type)), bact_type(bact_id_type), energy(ener) {}

	int id_room;
	int x;
	int y;
	int cell_type;
	int bact_type;
	int energy;
};

struct SaveBacteriumInfo : DataForSave
{
	explicit SaveBacteriumInfo(int id_r, int id, int r, int g, int b)
		:id_room(id_r), id_type(id), red(r), green(g), blue(b){}

	int id_room;
	int id_type;
	int red;
	int green;
	int blue;
};