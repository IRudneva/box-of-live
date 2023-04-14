#include "pch_server.h"
#include "client_packet.h"
#include "srv_manager.h"
#include "packet_writer.h"  
#include "network_server.h"
#include "log_duration.h"

#include "server_packet.h"

void SrvManager::handlePacket(const client_packet::PacketWithIdChannel& packet)
{
	switch (packet.packet->type)
	{
	case PacketType::CLI_CREATE_ROOM:
	{
		auto pt_cl = std::static_pointer_cast<client_packet::PTCreateRoom>(packet.packet);

		FieldStateInfo game_field_state;
		game_field_state.init(pt_cl->game_config);
		rooms_state_.insert({ last_id_room_, game_field_state });

		Logger::getInstance()->registerLog("SERVER::RECEIVED::CREATE ROOM::" + std::to_string(last_id_room_));

		const server_packet::PTNewRoom pt_new_room(last_id_room_, "room", game_field_state.getGameConfig());
		NetworkServer::getInstance()->sendPacketAllClients(pt_new_room);
		++last_id_room_;
		break;
	}
	case PacketType::CLI_CLOSE_ROOM:
	{
		auto pt_cl = std::static_pointer_cast<client_packet::PTCloseRoom>(packet.packet);

		Logger::getInstance()->registerLog("SERVER::RECEIVED::CLOSE ROOM::" + std::to_string(pt_cl->id_room));
	
		for (auto it = rooms_state_.begin(); it != rooms_state_.end();)
		{
			if (it->first == pt_cl->id_room)
				it = rooms_state_.erase(it);
			else
				++it;
		}

		server_packet::PTCloseRoom pt_close_room(static_cast<uint32_t>(pt_cl->id_room));
		NetworkServer::getInstance()->sendPacketAllClients(pt_close_room);

		break;
	}
	case PacketType::CLI_GET_ROOM_LIST:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::GET ROOM LIST");

		server_packet::PTRoomList pt_room_list;

		for (const auto& [id_room, state] : rooms_state_)
		{
			pt_room_list.room_list.emplace_back(static_cast<uint32_t>(id_room), "room", state.getGameConfig());
		}

		NetworkServer::getInstance()->sendPacket(packet.id_channel, pt_room_list);
		break;
	}
	case PacketType::CLI_CHOOSE_ROOM:
	{
		auto pt_choose_room = std::static_pointer_cast<client_packet::PTChooseRoom>(packet.packet);

		Logger::getInstance()->registerLog("SERVER::RECEIVED::CHOOSE ROOM::"+ std::to_string(pt_choose_room->id_room));

		for(auto& [id_room, state] : rooms_state_)
		{
			state.deleteSubscription(static_cast<int>(packet.id_channel));
		}
		rooms_state_.at(static_cast<int>(pt_choose_room->id_room)).addSubscription(static_cast<int>(packet.id_channel));
	
		const server_packet::PTInitChooseRoom pt_init_game(pt_choose_room->id_room, rooms_state_.at(static_cast<int>(pt_choose_room->id_room)).getGameConfig());
		NetworkServer::getInstance()->sendPacket(packet.id_channel, pt_init_game);
		break;
	}
	case PacketType::CLI_START_GAME:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::START GAMES");

		auto pt_st = std::static_pointer_cast<client_packet::PTStartGame>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).init(pt_st->game_config);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).reset();
		break;
	}
	case PacketType::MSG_DISABLE:
	{
		Logger::getInstance()->registerLog("SERVER::RECEIVED::MSG DISABLE");

		auto pt_st = std::static_pointer_cast<ConnectionMessage>(packet.packet);
		
		for (auto&[id_room, state] : rooms_state_)
		{
			state.deleteSubscription(static_cast<int>(packet.id_channel));
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

void SrvManager::updateGameState()
{
	if (NetworkServer::getInstance()->getConnectionCount() == 0)
		return;

	if (send_field_state_timer_.timedOut())
	{
		for (auto&[room, state] : rooms_state_)
		{
		//	LOG_DURATION("UPDATE GAME STATE::ROOM::" + std::to_string(room));
		//	if(state.isDeltaEmpty())
				state.update();
		}

		for (auto&[id_room, state] : rooms_state_)
		{
			if(state.getSubscription().empty())
				continue;

			const server_packet::PTRoomState game_state(id_room,
				state.getCellInfo(),
				state.getBacteriumInfo()/*, 
				state.getDeletedPosition()*/);

		//	state.clearDelta();

			for (const auto& client : state.getSubscription())
			{
			//	Logger::getInstance()->registerLog("SERVER::SEND::GAME STATE::CLIENT::" + std::to_string(client)+"::ID ROOM::"+std::to_string(id_room));
				
				NetworkServer::getInstance()->sendPacket(static_cast<uint32_t>(client), game_state);
			}
		}
	}
}