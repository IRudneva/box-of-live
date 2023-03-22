#pragma once
#include <map>

struct Room;
struct DeserializePacketWithIdChannel;

class SrvManager
{
public:
	const std::map<int, Room>& getRoomList() const { return room_list_; }

	void handlePacket(const DeserializePacketWithIdChannel& packet/*, const hv::SocketChannelPtr& channel*/);
private:
	std::map<int, Room> room_list_;
//	std::shared_ptr<SharedPacketQueue> packet_queue_ = std::make_shared<SharedPacketQueue>();
};
