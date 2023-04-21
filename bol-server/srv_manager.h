#pragma once
#include <map>
#include "field_state_info.h"
#include "bol_timer.h"

struct Room;
struct PacketWithIdChannel;

using IdRoom = int;
using IdChannel = int;

class SrvManager
{
public:
	void handlePacket(const client_packet::PacketWithIdChannel& packet);

	void initState(const std::map<int, GameConfig>& conf,const std::map<int, std::vector<SaveCellInfo>>& cell_inf, const std::map<int, std::vector<SaveBacteriumInfo>>& bact_inf)
	{
		std::lock_guard<std::mutex> lock(m_);

		for(const auto& [id_room, config] : conf)
		{
			FieldStateInfo fi;
			fi.init(std::make_shared<GameConfig>(config));

			for (const auto& info : cell_inf.at(id_room))
			{
				switch (static_cast<TypeCell>(info.cell_type))
				{
				case TypeCell::GRASS:
				{
					fi.addGrass(info.x, info.y);
					break;
				}
				case TypeCell::BACTERIUM:
				{
					for (const auto& bact : bact_inf.at(id_room))
					{
						if (bact.pos_x == info.x && bact.pos_y == info.y)
						{
							fi.addBacterium(bact.pos_x, bact.pos_y, bact.id_type, bact.energy);
						}
					}
					break;
				}
				}
			}
			rooms_state_[id_room] = fi;
			last_id_room_ = id_room;
		}
		last_id_room_++;
	}

	void updateGameState();

	std::map<int, GameConfig> formConfigForDatabase()
	{
		std::lock_guard<std::mutex> lock(m_);
		std::map<int, GameConfig> data;
		for(const auto& [id, state] : rooms_state_)
		{
			data.insert({ id, *state.getGameConfig() });
		}
		return data;
	}

	std::map<int, std::vector<SaveCellInfo>> formFieldsStateForDatabase()
	{
		std::lock_guard<std::mutex> lock(m_);
		std::map<int, std::vector<SaveCellInfo>> data;
		for (auto&[id_room, state] : rooms_state_)
		{
			for (const auto& cell_info : state.getAllCellInfo())
			{
				auto pos = cell_info.second->getPosition();
				auto type = cell_info.second->getCellType();
				data[id_room].push_back(SaveCellInfo(id_room, pos.x, pos.y, type));
			}
		}
		return data;
	}

	std::map<int, std::vector<SaveBacteriumInfo>> formBacteriumInfoForDatabase()
	{
		std::lock_guard<std::mutex> lock(m_);
		std::map<int, std::vector<SaveBacteriumInfo>> data;
		for (auto&[id_room, state] : rooms_state_)
		{
			for (const auto& bact : state.getAllBacteriumInfo())
			{
				data[id_room].push_back(SaveBacteriumInfo(id_room,
					bact.x,
					bact.y,
					bact.id_type,
					bact.energy,
					getRandomInt(0, 255),
					getRandomInt(0, 255),
					getRandomInt(0, 255)));
			}
		} 
		return data;
	}

private:
	std::mutex m_;
	std::map<IdRoom, FieldStateInfo> rooms_state_;
	int last_id_room_ = 0;
	Timer send_field_state_timer_{std::chrono::milliseconds(30)};

};