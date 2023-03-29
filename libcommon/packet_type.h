#pragma once

enum class PacketType : uint32_t
{
	CLI_CREATE_ROOM,
	SRV_NEW_ROOM,
	CLI_CLOSE_ROOM,
	SRV_CLOSE_ROOM,
	CLI_GET_ROOM_LIST,
	SRV_ROOM_LIST
};