#include "pch_server.h"
#include "client_packet.h"
#include "srv_manager.h"
#include "packet_writer.h"  
#include "network_server.h"

#include <iostream>

#include "server_packet.h"

void SrvManager::handlePacket(const client_packet::PacketWithIdChannel& packet)
{
	switch (packet.packet->type)
	{
	case PacketType::CLI_CREATE_ROOM:
	{
		std::cout << "room " << last_id_room_ << " created." << std::endl;

		FieldStateInfo game_field_state;
		rooms_state_.insert({ last_id_room_, game_field_state });

		server_packet::PTNewRoom pt_new_room(static_cast<uint32_t>(last_id_room_), "room");
		NetworkServer::getInstance().sendPacketAllClients(pt_new_room);
		++last_id_room_;
		break;
	}
	case PacketType::CLI_CLOSE_ROOM:
	{
		auto pt_cl = std::static_pointer_cast<client_packet::PTCloseRoom>(packet.packet);
		std::cout << "i received PTCloseRoom" << std::endl;

		for (auto it = room_subscription_.begin(); it != room_subscription_.end();)
		{
			if (it->first == pt_cl->id_room)
				it = room_subscription_.erase(it);
			else
				++it;
		}
		for (auto it = rooms_state_.begin(); it != rooms_state_.end();)
		{
			if (it->first == pt_cl->id_room)
				it = rooms_state_.erase(it);
			else
				++it;
		}

		server_packet::PTCloseRoom pt_close_room(static_cast<uint32_t>(pt_cl->id_room));
		NetworkServer::getInstance().sendPacketAllClients(pt_close_room);

		break;
	}
	case PacketType::CLI_GET_ROOM_LIST:
	{
		std::cout << "i received PTGetRoomList" << std::endl;

		server_packet::PTRoomList pt_room_list;

		for (const auto& id_room : getRoomList())
		{
			pt_room_list.room_list.emplace_back(static_cast<uint32_t>(id_room), "room");
		}

		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_room_list);
		break;
	}
	case PacketType::CLI_CHOOSE_ROOM:
	{
		std::cout << "i received ChooseRoom" << std::endl;

		auto pt_choose_room = std::static_pointer_cast<client_packet::PTChooseRoom>(packet.packet);
		
		for(auto& [id_room, id_channels] : room_subscription_)
		{
			id_channels.erase(std::remove(id_channels.begin(),id_channels.end(),static_cast<int>(packet.id_channel)), id_channels.end());
		}
		room_subscription_[static_cast<int>(pt_choose_room->id_room)].push_back(static_cast<int>(packet.id_channel));
	
		const server_packet::PTInitChooseRoom pt_init_game(pt_choose_room->id_room);
		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_init_game);
		break;
	}
	case PacketType::CLI_CHANGE_CONFIG:
	{
		std::cout << "i received PTChangeConfig" << std::endl;
		auto pt_change_config = std::static_pointer_cast<client_packet::PTChangeConfig>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_change_config->id_room)).init(pt_change_config->game_config);
		break;
	}
	case PacketType::CLI_START_GAME:
	{
		std::cout << "i received PTStartGame" << std::endl;
		auto pt_st = std::static_pointer_cast<client_packet::PTStartGame>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_st->id_room)).reset();
		break;
	}
	case PacketType::MSG_DISABLE:
	{
		std::cout << "i received MSG DISABLE" << std::endl;
		auto pt_st = std::static_pointer_cast<ConnectionMessage>(packet.packet);
		
		for (auto&[id_room, id_channels] : room_subscription_)
		{
			id_channels.erase(std::remove(id_channels.begin(), id_channels.end(), static_cast<int>(packet.id_channel)), id_channels.end());
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

const std::vector<IdRoom> SrvManager::getRoomList() const
{
	std::vector<IdRoom> room_list;
	for (const auto&[id, field_state] : rooms_state_)
	{
		room_list.push_back(id);
	}
	return room_list;
}

void SrvManager::updateGameState()
{
	if (NetworkServer::getInstance().getConnectionCount() == 0)
		return;

	if (send_field_state_timer_.timedOut())
	{
		for (auto&[room, state] : rooms_state_)
		{
			state.update();
			std::cout << "update room:  " << room << std::endl;
		}

		for (const auto&[id_room, clients] : room_subscription_)
		{
			if(clients.empty())
				continue;

			const server_packet::PTRoomState state(id_room,
				rooms_state_.at(id_room).getCellInfo(),
				rooms_state_.at(id_room).getBacteriumInfo());

			for (const auto& client : clients)
			{
				std::cout << "send state cli:  " << client << "  id room:  " << id_room << std::endl;
				NetworkServer::getInstance().sendPacket(static_cast<uint32_t>(client), state);
			}
		}
	}
}