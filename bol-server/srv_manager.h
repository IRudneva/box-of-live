#pragma once
#include <hv/Channel.h>
#include "srv_shared_packet_queue.h"

class SrvManager
{
public:
	const std::map<int, Room>& getRoomList() const { return room_list_; }

	void handlePacket(std::shared_ptr<DeserializePacket> packet, const hv::SocketChannelPtr& channel);
private:
	std::map<int, Room> room_list_;
	std::shared_ptr<SharedPacketQueue> packet_queue_ = std::make_shared<SharedPacketQueue>();
};