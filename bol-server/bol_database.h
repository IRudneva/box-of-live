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
			std::cout << e.what() << std::endl;
		}
	}

	std::map<int, GameConfig> getConfigData()
	{
		std::map<int, GameConfig> result;
		try {
			db_ << "SELECT * FROM config_for_room" >> [&](int id_room,
				int energy_base,
				int energy_action_cost,
				int energy_to_clone,
				int min_update_time,
				int max_update_time,
				int grass_update_time,
				int count_grass,
				int energy_from_grass,
				int delta_game_field_size) {result[id_room] = GameConfig(energy_base, energy_action_cost, energy_to_clone, min_update_time,
					max_update_time, grass_update_time, count_grass, energy_from_grass, delta_game_field_size
				); };
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
			return result;
		

	}
	std::map<int, std::vector<SaveCellInfo>> getFieldStateData()
	{
		std::map<int, std::vector<SaveCellInfo>> result;
		try {
			db_ << "SELECT * FROM rooms_state" >> [&](int id_room, int x, int y, int cell_type)
			{
				result[id_room].push_back(SaveCellInfo(id_room, x, y, static_cast<TypeCell>(cell_type)));
			};
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		return result;
	}

	std::map<int, std::vector<SaveBacteriumInfo>> getBacteriumData()
	{
		std::map<int, std::vector<SaveBacteriumInfo>> result;
		try {
			db_ << "SELECT * FROM bacterium_state" >> [&](int id_room, int x, int y, int type, int energy, int r, int g, int b)
			{
				result[id_room].push_back(SaveBacteriumInfo(id_room, x, y, type, energy, r, g, b));
			};
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		return result;
	}

	void saveConfig(const std::map<int, GameConfig>& data)
	{
		clearDataConfig();
		for (const auto&[id_room, d] : data)
		{
			try
			{
				db_ << "INSERT INTO config_for_room "
					"(id_room, "
					"energy_base, "
					"energy_action_cost, "
					"energy_to_clone, "
					"min_update_time, "
					"max_update_time, "
					"grass_update_time, "
					"count_grass, "
					"energy_from_grass, "
					"delta_game_field_size)"
					" VALUES "
					"(?,?,?,?,?,?,?,?,?,?)"
					<< id_room
					<< d.energy_base
					<< d.energy_action_cost
					<< d.energy_to_clone
					<< d.min_update_time
					<< d.max_update_time
					<< d.grass_update_time
					<< d.count_grass
					<< d.energy_from_grass
					<< d.delta_game_field_size;
			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}
	}

	void saveFieldsState(const std::map<int, std::vector<SaveCellInfo>>& data)
	{
		clearDataFieldsState();
		for (const auto&[id_room, cell_inf] : data)
		{
			try
			{
				for (const auto& ci : cell_inf)
				{
					db_ << "INSERT INTO rooms_state (id_room, pos_x, pos_y, type_cell) VALUES (?,?,?,?)"
						<< ci.id_room
						<< ci.x
						<< ci.y
						<< ci.cell_type;
				}
			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}

	}
	void saveBacteruium(const std::map<int, std::vector<SaveBacteriumInfo>>& data)
	{
		clearDataBacterium();
		for (const auto&[id_room, cell_inf] : data)
		{
			try
			{
				for (const auto& ci : cell_inf)
				{
					db_ << "INSERT INTO bacterium_state (id_room, pos_x, pos_y, id_type, energy, red, green, blue) VALUES (?,?,?,?,?,?,?,?)"
						<< id_room
						<< ci.pos_x
						<< ci.pos_y
						<< ci.id_type
						<< ci.energy
						<< ci.red
						<< ci.green
						<< ci.blue;
				}
			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}
	}


private:
	database db_;

	void createTable()
	{
		db_ << "CREATE TABLE IF NOT EXISTS rooms_state ("
			"id_room INTEGER,"	
			"pos_x INTEGER,"
			"pos_y INTEGER,"
			"type_cell INTEGER"
			");";

		db_ << "CREATE TABLE IF NOT EXISTS bacterium_state ("
			"id_room INTEGER,"
			"pos_x INTEGER,"
			"pos_y INTEGER,"
			"id_type INTEGER,"
			"energy INTEGER,"
			"red INTEGER,"
			"green INTEGER,"
			"blue INTEGER"
			");";

		db_ << "CREATE TABLE IF NOT EXISTS config_for_room ("
			"id_room INTEGER PRIMARY KEY,"
			"energy_base INTEGER,"
			"energy_action_cost INTEGER,"
			"energy_to_clone INTEGER,"
			"min_update_time  INTEGER,"
			"max_update_time INTEGER,"
			"grass_update_time INTEGER,"
			"count_grass INTEGER,"
			"energy_from_grass INTEGER,"
			"delta_game_field_size INTEGER"
			");";
	}


	void clearDataConfig()
	{
		try
		{
			db_ << "DELETE FROM config_for_room;";
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	void clearDataFieldsState()
	{
		try
		{
			db_ << "DELETE FROM rooms_state;";
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	void clearDataBacterium()
	{
		try
		{
			db_ << "DELETE FROM bacterium_state;";
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
};
