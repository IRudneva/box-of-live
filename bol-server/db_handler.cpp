#include "pch_server.h"
#include "db_handler.h"

DbHandler::DbHandler() : db_("bol_database.db")
{
	DbPayload::getInstance()->initDb(std::make_shared<database>(db_));
	try
	{
		createTable();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << " line: " << __LINE__ << std::endl;
	}
}

std::map<int, DbRoomInfo> DbHandler::getRoomsData()
{
	std::map<int, DbRoomInfo> result;
	try {
		db_ << "SELECT * FROM room" >> [&](
			int id_room,
			std::string name,
			int is_active,
			int conf_energy_base,
			int conf_energy_action_cost,
			int conf_energy_to_clone,
			int conf_count_colonies,
			int conf_count_bacterium,
			int conf_min_update_time,
			int conf_max_update_time,
			int conf_grass_update_time,
			int conf_count_grass,
			int conf_energy_from_grass,
			int conf_delta_game_field_size) {
			result[id_room] = DbRoomInfo(static_cast<bool>(is_active), GameConfig(conf_energy_base, conf_energy_action_cost, conf_energy_to_clone,
				conf_count_colonies, conf_count_bacterium, conf_min_update_time,
				conf_max_update_time, conf_grass_update_time, conf_count_grass,
				conf_energy_from_grass, conf_delta_game_field_size
			));
		};
	}
	catch (const std::exception& e) {
		std::cout << e.what() << " line: " << __LINE__ << std::endl;
	}
	return result;
}

using XYPos = std::pair<int, int>;
std::map<int, std::unordered_map<XYPos, DbCellState, pairhash>> DbHandler::getFieldStateData()
{
	std::map<int, std::unordered_map<XYPos, DbCellState, pairhash>> result;
	try {
		db_ << "SELECT * FROM room_cells" >> [&](int id_room, int pos_x, int pos_y, int type_cell, int grass_type, int bact_type, int energy)
		{
			XYPos key = { pos_x, pos_y };
			DbCellState state{ static_cast<TypeCell>(type_cell), static_cast<bool>(grass_type), bact_type, energy };
			result[id_room].insert({ key, state });
			
		};
	}
	catch (const std::exception& e) {
		std::cout << e.what() << " line: " << __LINE__ << std::endl;
	}
	return result;
}

std::vector<DbBacteriumColorState> DbHandler::getBacteriumData()
{
	std::vector<DbBacteriumColorState> result;
	try {
		db_ << "SELECT * FROM bacterium_color" >> [&](int id_room, int bact_type, int red, int green, int blue)
		{
			result.push_back(DbBacteriumColorState(id_room, bact_type, red, green, blue));
		};
	}
	catch (const std::exception& e) {
		std::cout << e.what() << " line: " << __LINE__ << std::endl;
	}
	return result;
}

void DbHandler::createTable()
{
	db_ << "CREATE TABLE IF NOT EXISTS room ("
		"id_room INTEGER PRIMARY KEY,"
		"name TEXT,"
		"is_active INTEGER,"
		"conf_energy_base INTEGER,"
		"conf_energy_action_cost INTEGER,"
		"conf_energy_to_clone INTEGER,"
		"conf_count_colonies INTEGER,"
		"conf_count_bacterium INTEGER,"
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
		"grass_type INTEGER,"
		"bact_type INTEGER,"
		"energy INTEGER,"
		"PRIMARY KEY(id_room, pos_x, pos_y)"
		");";
}