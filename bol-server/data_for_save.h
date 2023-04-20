#pragma once
#include "game_domain.h"

struct DataForSave
{
	virtual ~DataForSave() = default;
protected:
	DataForSave() = default;
};

//struct DataForTabRooms : public DataForSave
//{
//	explicit DataForTabRooms(int id) : id_room(id){}
//	int id_room;
//};

struct DataForTabRoomsState : public DataForSave
{
	explicit DataForTabRoomsState(int id, int ox, int oy, TypeCell type)
	: id_room(id), x(ox), y(oy), cell_type(static_cast<int>(type)) {}

	int id_room;
	int x;
	int y;
	int cell_type;
};