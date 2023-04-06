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
	const std::vector<IdRoom> getRoomList() const
	{
		std::vector<IdRoom> room_list;
		for(const auto& [id, field_state] : rooms_state_)
		{
			room_list.push_back(id);
		}
		return room_list;
	}

	void handlePacket(const client_packet::PacketWithIdChannel& packet);

	void updateGameState();

private:
	std::map<IdRoom, FieldStateInfo> rooms_state_;
	std::map<IdRoom, std::vector<IdChannel>> room_subscription_;
	int last_id_room_ = 0;
	Timer send_field_state_timer_{0.5};
};
