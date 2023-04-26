#pragma once
#include "packet.h"
#include "packet_domain.h"
#include "game_domain.h"
#include "msgpack.hpp"
#include <vector>

namespace server_packet {
	struct ServerPacket : Packet
	{
	protected:
		ServerPacket(PacketType ty) : Packet(ty) {}
	public:
		void pack(msgpack::Packer& packer) const  override {}
		void pack(msgpack::Unpacker& unpacker) override {}
	};

	struct PTRoomList : ServerPacket
	{
		PTRoomList() :ServerPacket(PacketType::SRV_ROOM_LIST) {}
		PTRoomList(const std::vector<Room>& rl) :ServerPacket(PacketType::SRV_ROOM_LIST), room_list(rl) {}
		std::vector<Room> room_list;

		void pack(msgpack::Packer& packer) const  override { packer(room_list); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(room_list); }
	};

	struct PTNewRoom : ServerPacket
	{
		PTNewRoom() : ServerPacket(PacketType::SRV_NEW_ROOM){}
		PTNewRoom(const Room& other) : ServerPacket(PacketType::SRV_NEW_ROOM), room(other) {}
		PTNewRoom(uint32_t id_room, const std::string& name, std::shared_ptr<GameConfig> conf) :ServerPacket(PacketType::SRV_NEW_ROOM), room(id_room,name, std::move(conf)) {}
		Room room;
		void pack(msgpack::Packer& packer) const  override { packer(room); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(room); }
	};

	struct PTCloseRoom : ServerPacket
	{
		PTCloseRoom() : ServerPacket(PacketType::SRV_CLOSE_ROOM) {}
		PTCloseRoom(uint32_t id) : ServerPacket(PacketType::SRV_CLOSE_ROOM), id_room(id){}
		uint32_t id_room = 0;
		void pack(msgpack::Packer& packer) const  override { packer(id_room); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room); }
	};

	struct PTRoomState : ServerPacket
	{
		PTRoomState() : ServerPacket(PacketType::SRV_ROOM_STATE) {}
		PTRoomState(uint32_t id, const std::vector<GrassInfo>& g_info, const std::vector<BacteriumInfo>& bact_info, const std::vector<DeletedPosition>& del_pos)
		: ServerPacket(PacketType::SRV_ROOM_STATE),
		id_room(id),
		grass_info(g_info),
		bacterium_info(bact_info),
		deleted_position(del_pos) {}

		uint32_t id_room = 0;
		std::vector<GrassInfo> grass_info = {};
		std::vector<BacteriumInfo> bacterium_info = {};
		std::vector<DeletedPosition> deleted_position = {};
		void pack(msgpack::Packer& packer) const override { packer(id_room, grass_info, bacterium_info, deleted_position); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room, grass_info, bacterium_info, deleted_position); }
	};

	struct PTInitChooseRoom : ServerPacket
	{
		PTInitChooseRoom() :ServerPacket(PacketType::SRV_INIT_CHOOSE_ROOM) {}
		PTInitChooseRoom(uint32_t id, const std::vector<GrassInfo>& g_info, const std::vector<BacteriumInfo>& bact_info, std::shared_ptr<GameConfig> conf, std::map<int, SrvColor> color)
		:ServerPacket(PacketType::SRV_INIT_CHOOSE_ROOM), id_room(id), config(std::move(conf)),
		 grass_info(g_info), bacterium_info(bact_info), bacterium_color_by_type(color) {}

		uint32_t id_room = 0;
		std::shared_ptr<GameConfig> config = std::make_shared<GameConfig>();
		std::vector<GrassInfo> grass_info = {};
		std::vector<BacteriumInfo> bacterium_info = {};
		std::map<int, SrvColor> bacterium_color_by_type = {};

		void pack(msgpack::Packer& packer) const  override { packer(id_room, *config, grass_info, bacterium_info, bacterium_color_by_type); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room, *config, grass_info, bacterium_info, bacterium_color_by_type); }
	};
} // namespace server_packet