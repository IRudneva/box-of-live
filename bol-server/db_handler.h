#pragma once
#include <iostream>
#include <mutex>
#include <queue>
#include "db_payload.h"

using namespace sqlite;

class DbHandler
{
public:
	explicit DbHandler();

	std::map<int, DbRoomInfo> getRoomsData();

	using XYPos = std::pair<int, int>;
	std::map<int, std::unordered_map<XYPos, DbCellState, pairhash>> getFieldStateData();

	std::vector<DbBacteriumColorState> getBacteriumData();

	void saveData() const
	{
		DbPayload::getInstance()->save();
	}
private:
	database db_;

	void createTable();
};
