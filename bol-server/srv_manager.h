#pragma once
#include <map>
#include "field_state_info.h"

struct Room;
struct PacketWithIdChannel;

class SrvManager
{
public:
	const std::map<int, Room>& getRoomList() const { return room_list_; }

	void handlePacket(const client_packet::PacketWithIdChannel& packet);

private:
	using IdRoom = int;
	std::map<IdRoom, Room> room_list_;
	std::map<IdRoom, FieldStateInfo> rooms_state_;
	int last_id_room_ = 0;
};
