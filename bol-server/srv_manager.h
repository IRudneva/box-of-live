#pragma once
#include <map>

struct Room;
struct PacketWithIdChannel;

class SrvManager
{
public:
	const std::map<int, Room>& getRoomList() const { return room_list_; }

	void handlePacket(const client_packet::PacketWithIdChannel& packet);

private:
	std::map<int, Room> room_list_;
	int last_id_room_ = 0;
};
