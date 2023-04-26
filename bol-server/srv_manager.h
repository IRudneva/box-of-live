#pragma once
#include <map>
#include "db_handler.h"
#include "room_state.h"
#include "bol_timer.h"

struct Room;
struct PacketWithIdChannel;

using IdRoom = int;
using IdChannel = int;

class SrvManager
{
public:
	static SrvManager* getInstance()
	{
		static SrvManager inst;
		return  &inst;
	}

	void handlePacket(const client_packet::PacketWithIdChannel& packet);

	using XYPos = std::pair<int, int>;
	void initState(const std::map<int, DbRoomInfo>& rooms_state, const std::map<int, std::unordered_map<XYPos, DbCellState, pairhash>>& cell_inf, const std::map<int, std::vector<DbBacteriumColorState>>& bact_inf);

	void updateGameState();


private:
	//std::mutex m_;
	std::map<IdRoom, RoomState> rooms_state_;
	int last_id_room_ = 0;

	SrvManager(const SrvManager&) = delete;
	SrvManager& operator=(SrvManager&) = delete;
	SrvManager() = default;
	~SrvManager() = default;
};