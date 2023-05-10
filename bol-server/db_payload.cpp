#include "pch_server.h"
#include "db_payload.h"

#include <utility>

#include "logger.h"

DbPayload*  DbPayload::getInstance()
{
	static DbPayload inst;
	return &inst;
}

void DbPayload::initDb(std::shared_ptr<sqlite::database> p_db)
{
	std::lock_guard<std::mutex> lock(m_);
	p_db_ = std::move(p_db);
}

void DbPayload::save()
{
	{
		std::lock_guard<std::mutex> lock(m_);
		*p_db_ << "begin;";

		for (const auto&[room_id, info] : rooms_info_)
		{
			try {
					*p_db_ << "INSERT OR REPLACE INTO room "
						"(id_room,"
						"name,"
						"is_active,"
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
						"(?,?,?,?,?,?,?,?,?,?,?,?);"
						<< room_id
						<< "room"
						<< static_cast<int>(info.is_active)
						<< info.config.energy_base
						<< info.config.energy_action_cost
						<< info.config.energy_to_clone
						<< info.config.min_update_time
						<< info.config.max_update_time
						<< info.config.grass_update_time
						<< info.config.count_grass
						<< info.config.energy_from_grass
						<< info.config.delta_game_field_size;
			}
			catch (const std::exception& e) {
				std::cout << e.what() << " line: " << __LINE__ << std::endl;
			}
		}

		for (const auto& color_states : bacterium_color_states_)
		{
			try {
				*p_db_ << "INSERT OR REPLACE INTO bacterium_color "
					"(id_room,"
					"bact_type,"
					"red,"
					"green,"
					"blue)"
					" VALUES "
					"(?,?,?,?,?);"
					<< color_states.id_room
					<< color_states.id_type
					<< color_states.red
					<< color_states.green
					<< color_states.blue;
			}
			catch (const std::exception& e) {
				std::cout << e.what() << " line: " << __LINE__ << std::endl;
			}
		}

		for (const auto &[room_id, room_state] : cells_room_states_) {
			if (room_state.is_deleted) {
				try
				{
					std::cout << "DELETE ROOM " << room_id << std::endl;

					*p_db_ << "DELETE FROM room WHERE id_room = (?);" << room_id;

					*p_db_ << "DELETE FROM room_cells WHERE id_room = (?);" << room_id;

					*p_db_ << "DELETE FROM bacterium_color WHERE id_room = (?);" << room_id;
				}
				catch (const std::exception& e) {
					std::cout << e.what() << " line: " << __LINE__ << std::endl;
				}
				continue;
				// запрос на удаление комнаты из всех таблиц
			}

			for (const auto &[cell_pos, cell_state] : room_state.cell_states) {
				if (cell_state.cell_type == TypeCell::EMPTY) {
					try
					{
						*p_db_ << "DELETE FROM room_cells WHERE id_room = (?) AND pos_x = (?) AND pos_y = (?);" << room_id << cell_pos.first << cell_pos.second;
					}
					catch (const std::exception& e) {
						std::cout << e.what() << " line: " << __LINE__ << std::endl;
					}
					continue;
				}

				try
				{
					*p_db_ << "INSERT OR REPLACE INTO room_cells (id_room, pos_x, pos_y, type_cell, grass_type, bact_type, energy) VALUES (?,?,?,?,?,?,?);"
						<< room_id
						<< cell_pos.first
						<< cell_pos.second
						<< static_cast<int>(cell_state.cell_type)
						<< static_cast<int>(cell_state.is_super_grass)
						<< cell_state.bact_type
						<< cell_state.energy;
				}
				catch (const std::exception& e) {
					std::cout << e.what() << " line: " << __LINE__ << std::endl;
				}
			}
		}
		*p_db_ << "commit;";
		clear();
	} // lock
}

void DbPayload::updateRoomsConfigInfo(int id, const DbRoomInfo& inf)
{
	std::lock_guard<std::mutex> lock(m_);
	if (rooms_info_.find(id) == rooms_info_.end())
		rooms_info_.insert({ id, inf });

	rooms_info_.at(id) = inf;
}

void DbPayload::updateCellsRoomState(const DbSaveRoomState& delta_info)
{
	{
		std::lock_guard<std::mutex> lock(m_);

		auto current_room = cells_room_states_.find(delta_info.id_room);

		if (current_room != cells_room_states_.end())
		{
			cells_room_states_.at(delta_info.id_room).is_deleted = delta_info.is_deleted;

			if (delta_info.is_deleted)
				return;

			if (delta_info.cell_states.empty()) //если нечего обновлять 
				return;

			for (const auto&[new_pos, new_cell] : delta_info.cell_states)
			{
				cells_room_states_.at(delta_info.id_room).cell_states[new_pos] = new_cell;
			}
		}
		else
		{
			cells_room_states_.insert({ delta_info.id_room, delta_info });
		}
	}//lock
}

void DbPayload::updateBacteriumColorStates(std::vector<DbBacteriumColorState>& inf)
{
	std::lock_guard<std::mutex> lock(m_);
	bacterium_color_states_= inf;
}

void DbPayload::clear()
{
	rooms_info_.clear();
	cells_room_states_.clear();
	bacterium_color_states_.clear();
}