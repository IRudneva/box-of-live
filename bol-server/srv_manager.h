#pragma once
#include <map>
#include "db_handler.h"
#include "room_state.h"

struct Room;
struct PacketWithIdChannel;

using IdRoom = int;
using IdChannel = int;

class SrvManager
{
public:
	void handlePacket(const client_packet::PacketWithIdChannel& packet);

	using XYPos = std::pair<int, int>;
	void initState(const std::map<int, DbRoomInfo>& rooms_state, const std::map<int, std::unordered_map<XYPos, DbCellState, pairhash>>& cell_inf, const std::vector<DbBacteriumColorState>& bact_inf);

	void updateGameState();
private:
	std::map<IdRoom, RoomState> rooms_state_;
	int last_id_room_ = 0;

	RoomState createRoom(int id, bool status, const GameConfig& config,
		const std::map<int, SrvColor>& color_bacterium) const;

	RoomState createRoom(int id, bool status, std::shared_ptr<GameConfig> config) const;
};