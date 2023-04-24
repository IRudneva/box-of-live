#pragma once
#include "bol_database.h"
#include "data_for_save.h"
#include "field_state.h"

class RequestHandler
{
public:
	static RequestHandler* getInstance()
	{
		static RequestHandler inst;
		return &inst;
	}

	void init(std::shared_ptr<DatabaseHandler> db)
	{
		handler_ = db;
	}

	void saveNewRoom(int id_room, const RoomInfo& info)
	{
		handler_->saveRoom(id_room, info);
	}

	void deleteRoom(int id_room)
	{
		handler_->deleteRoom(id_room);
	}

	void changeRoomStatus(int id, bool st)
	{
		handler_->updateRoomStatus("UPDATE room SET status = (?) WHERE id_room = (?)", id, st);
	}

	void changeRoomConfig(int id, const GameConfig& conf)
	{
		handler_->updateRoomConfig("UPDATE room "
			"SET conf_energy_base = (?),"
			"conf_energy_action_cost = (?),"
			"conf_energy_to_clone = (?),"
			"conf_min_update_time = (?),"
			"conf_max_update_time = (?),"
			"conf_grass_update_time = (?),"
			"conf_count_grass = (?),"
			"conf_energy_from_grass = (?),"
			"conf_delta_game_field_size = (?)"
			" WHERE id_room = (?)", id, conf);
	}

	void updateRoomCells(int id_room, const std::vector<DeletedPosition>& del_inf, const std::vector<GrassInfo>& grass_inf, const std::vector<BacteriumInfo>& bact_inf)
	{
		for(const auto& pos : del_inf)
		{
			handler_->deleteRowInTabRoomState(id_room, pos.x, pos.y);
		}

		std::vector<SaveCellInfo> data;
		for(const auto& grass : grass_inf)
		{
			data.push_back(SaveCellInfo(id_room, grass.x, grass.y, TypeCell::GRASS, 0, 0));
		}

		for (const auto& bact : bact_inf)
		{
			data.push_back(SaveCellInfo(id_room, bact.x, bact.y, TypeCell::BACTERIUM, bact.id_type, bact.energy));
		}

		handler_->saveRoomState(data);
	}

private:
	std::shared_ptr<DatabaseHandler> handler_;

	RequestHandler(const RequestHandler&) = delete;
	RequestHandler& operator=(RequestHandler&) = delete;
	RequestHandler() = default;
	~RequestHandler() = default;
};

