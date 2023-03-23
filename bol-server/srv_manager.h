#pragma once
#include <map>

struct Room;
struct DeserializePacketWithIdChannel;

class SrvManager
{
public:
	const std::map<int, Room>& getRoomList() const { return room_list_; }

	void handlePacket(const DeserializePacketWithIdChannel& packet);
private:
	std::map<int, Room> room_list_;
};
