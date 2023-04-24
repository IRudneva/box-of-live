#pragma once
#include <mutex>
#include <queue>
#include "data_for_save.h"

using namespace  sqlite;

class DatabaseHandler
{
public:
	explicit DatabaseHandler(database db) : db_(std::move(db))
	{
		try
		{
			createTable();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
	}

	std::map<int, RoomInfo> getRoomsData()
	{
		std::map<int, RoomInfo> result;
		try {
			db_ << "SELECT * FROM room" >> [&](
				int id_room,
				std::string name,
				std::string status,
				int conf_energy_base,
				int conf_energy_action_cost,
				int conf_energy_to_clone,
				int conf_min_update_time,
				int conf_max_update_time,
				int conf_grass_update_time,
				int conf_count_grass,
				int conf_energy_from_grass,
				int conf_delta_game_field_size) {
				bool is_active = (status == "active" ? true : false);
				result[id_room] = RoomInfo(is_active, GameConfig(conf_energy_base, conf_energy_action_cost, conf_energy_to_clone, conf_min_update_time,
					conf_max_update_time, conf_grass_update_time, conf_count_grass, conf_energy_from_grass, conf_delta_game_field_size
				));
			};
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
			return result;
		

	}

	std::map<int, std::vector<SaveCellInfo>> getFieldStateData()
	{
		std::map<int, std::vector<SaveCellInfo>> result;
		try {
			db_ << "SELECT * FROM room_cells" >> [&](int id_room, int pos_x, int pos_y, int type_cell, int bact_type, int energy)
			{
				result[id_room].push_back(SaveCellInfo(id_room, pos_x, pos_y, static_cast<TypeCell>(type_cell), bact_type, energy));
			};
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
		return result;
	}

	std::map<int, std::vector<SaveBacteriumInfo>> getBacteriumData()
	{
		std::map<int, std::vector<SaveBacteriumInfo>> result;
		try {
			db_ << "SELECT * FROM bacterium_color" >> [&](int id_room, int bact_type, int red, int green, int blue)
			{
				result[id_room].push_back(SaveBacteriumInfo(id_room, bact_type, red, green, blue));
			};
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
		return result;
	}

	void saveRoom(/*const std::map<int, RoomInfo>& data*/int id_room, const RoomInfo& inf)
	{
	/*	for (const auto&[id_room, inf] : data)
		{*/
			try
			{
				std::string status = (inf.is_active ? "active" : "inactive");
				db_ << "INSERT INTO room "
					"(id_room,"
					"name,"
					"status,"
					"conf_energy_base,"
					"conf_energy_action_cost,"
					"conf_energy_to_clone,"
					"conf_min_update_time,"
					"conf_max_update_time,"
					"conf_grass_update_time,"
					"conf_count_grass,"
					"conf_energy_from_grass,"
					"conf_delta_game_field_size)"
					" VALUES "
					"(?,?,?,?,?,?,?,?,?,?,?,?)"
					<< id_room
					<< "room"
					<< status
					<< inf.config.energy_base
					<< inf.config.energy_action_cost
					<< inf.config.energy_to_clone
					<< inf.config.min_update_time
					<< inf.config.max_update_time
					<< inf.config.grass_update_time
					<< inf.config.count_grass
					<< inf.config.energy_from_grass
					<< inf.config.delta_game_field_size;
			}
			catch (const std::exception& e) {
				std::cout << e.what() << " line: " << __LINE__ << std::endl;
			}
	//	}
	}

	void deleteRoom(int id)
	{
		try
		{
			db_ << "DELETE FROM room WHERE id_room = (?)" << id;

			db_ << "DELETE FROM room_cells WHERE id_room = (?)" << id;

			db_ << "DELETE FROM bacterium_color WHERE id_room = (?)" << id;
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
	}

	void updateRoomStatus(const std::string& request, int id, bool st)
	{
		try
		{
			std::string status = (st ? "active" : "inactive");
			db_ << request << status << id;
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
	}

	void updateRoomConfig(const std::string& request, int id, const GameConfig& config)
	{
		try
		{
		db_ << request
			<< config.energy_base
			<< config.energy_action_cost
			<< config.energy_to_clone
			<< config.min_update_time
			<< config.max_update_time
			<< config.grass_update_time
			<< config.count_grass
			<< config.energy_from_grass
			<< config.delta_game_field_size
			<< id;
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
	}

	void saveRoomState(const std::vector<SaveCellInfo>& data)
	{
		try
		{
			for (const auto& ci : data)
			{
				db_ << "INSERT OR REPLACE INTO room_cells (id_room, pos_x, pos_y, type_cell, bact_type, energy) VALUES (?,?,?,?,?,?)"
					<< ci.id_room
					<< ci.x
					<< ci.y
					<< ci.cell_type
					<< ci.bact_type
					<< ci.energy;
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
	}

	void deleteRowInTabRoomState(int id_room, int x, int y)
	{
		try
		{
			db_ << "DELETE FROM room_cells WHERE id_room = (?) AND pos_x = (?) AND pos_y = (?)" << id_room << x << y;
		}
		catch (const std::exception& e) {
			std::cout << e.what() << " line: " << __LINE__ << std::endl;
		}
	}

	//void saveBacteruium(int id_room, const std::vector<SaveBacteriumInfo> data)
	//{
	//	for (const auto&[id_room, cell_inf] : data)
	//	{
	//		try
	//		{
	//			for (const auto& ci : cell_inf)
	//			{
	//				db_ << "INSERT OR REPLACE INTO bacterium_color (id_room, bact_type, red, green, blue) VALUES (?,?,?,?,?)"
	//					<< id_room
	//					<< ci.id_type
	//					<< ci.red
	//					<< ci.green
	//					<< ci.blue;
	//			}
	//		}
	//		catch (const std::exception& e) {
	//			std::cout << e.what() << " line: " << __LINE__ << std::endl;
	//		}
	//	}
	//}


private:
	database db_;

	void createTable()
	{
		db_ << "CREATE TABLE IF NOT EXISTS room ("
			"id_room INTEGER PRIMARY KEY,"
			"name TEXT,"
			"status TEXT,"
			"conf_energy_base INTEGER,"
			"conf_energy_action_cost INTEGER,"
			"conf_energy_to_clone INTEGER,"
			"conf_min_update_time  INTEGER,"
			"conf_max_update_time INTEGER,"
			"conf_grass_update_time INTEGER,"
			"conf_count_grass INTEGER,"
			"conf_energy_from_grass INTEGER,"
			"conf_delta_game_field_size INTEGER"
			");";

		db_ << "CREATE TABLE IF NOT EXISTS bacterium_color ("
			"id_room INTEGER,"
			"bact_type INTEGER,"
			"red INTEGER,"
			"green INTEGER,"
			"blue INTEGER,"
			"PRIMARY KEY(id_room, bact_type)"
			");";

		db_ << "CREATE TABLE IF NOT EXISTS room_cells ("
			"id_room INTEGER,"
			"pos_x INTEGER,"
			"pos_y INTEGER,"
			"type_cell INTEGER,"
			"bact_type INTEGER,"
			"energy INTEGER,"
			"PRIMARY KEY(id_room, pos_x, pos_y)"
			");";

	}
};
