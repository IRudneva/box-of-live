#pragma once
#include <map>
#include "bol_database.h"
#include "room_state.h"
#include "bol_timer.h"

struct Room;
struct PacketWithIdChannel;

using IdRoom = int;
using IdChannel = int;

class SrvManager
{
public:
	void handlePacket(const client_packet::PacketWithIdChannel& packet);

	void initState(const std::map<int, RoomInfo>& rooms_state, const std::map<int, std::vector<SaveCellInfo>>& cell_inf, const std::map<int, std::vector<SaveBacteriumInfo>>& bact_inf)
	{
		//std::lock_guard<std::mutex> lock(m_);

		for(const auto& [id_room, room_info] : rooms_state)
		{
			RoomState fi(id_room, room_info.is_active);
			fi.init(std::make_shared<GameConfig>(room_info.config));

			if (cell_inf.find(id_room) != cell_inf.end())
			{
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
						fi.addBacterium(info.x, info.y, info.bact_type, info.energy);
						break;
					}
					}
				}
			}
			rooms_state_[id_room] = fi;
			last_id_room_ = id_room;
		}
		if(!rooms_state.empty())
			++last_id_room_;
	}

	void updateGameState();


private:
	std::mutex m_;
	std::map<IdRoom, RoomState> rooms_state_;
	int last_id_room_ = 0;
	Timer send_field_state_timer_{std::chrono::milliseconds(30)};

};