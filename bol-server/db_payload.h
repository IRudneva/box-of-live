#pragma once
#include <mutex>
#include "data_for_save.h"

class DbPayload
{
public:
	static DbPayload* getInstance();

	void initDb(std::shared_ptr<sqlite::database> p_db);

	void save();

	void updateRoomsConfigInfo(int id, const DbRoomInfo& inf);

	void updateCellsRoomState(const DbSaveRoomState& inf);

	void updateBacteriumColorStates(std::vector<DbBacteriumColorState>& inf);

	void clear();

private:
	std::mutex m_;
	std::shared_ptr<sqlite::database> p_db_;

	using idRoom = int;
	std::map<idRoom, DbRoomInfo> rooms_info_;
	std::map<idRoom, DbSaveRoomState> cells_room_states_;
	std::vector<DbBacteriumColorState> bacterium_color_states_;

	DbPayload(const DbPayload&) = delete;
	DbPayload& operator=(DbPayload&) = delete;
	DbPayload() = default;
	~DbPayload() = default;
};

