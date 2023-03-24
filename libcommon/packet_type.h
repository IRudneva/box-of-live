#pragma once

enum class PacketType : uint32_t
{
	PT_CREATE_ROOM,
	PT_CLOSE_ROOM,
	PT_GET_ROOM_LIST,
	PT_ROOM_LIST,

	NO_TYPE
};