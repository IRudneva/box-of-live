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

	void updateGameState();

	void initRoomList()
	{
		auto rooms = QueueRequest::getInstance()->GetAllRooms("SELECT DISTINCT id_room FROM rooms_state");
		auto all_states = QueueRequest::getInstance()->GetStates("SELECT * FROM rooms_state");

		for(const auto r: rooms)
		{
			FieldStateInfo fs;
			for(const auto& st:all_states)
			{
				if(r == st.id_room)
				{
					fs.setState(st.x, st.y, static_cast<TypeCell>(st.cell_type));
				}
			}
			rooms_state_.insert({ r, fs});
		}
	}

	void formDataForDatabase();

private:
	std::map<IdRoom, FieldStateInfo> rooms_state_;
	int last_id_room_ = 0;
	Timer send_field_state_timer_{std::chrono::milliseconds(30)};

};