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
		std::cout << "i received PTCreateRoom" << std::endl;

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
		auto client = std::find(room_subscription_[pt_cl->id_room].begin(), room_subscription_[pt_cl->id_room].end(), packet.id_channel);
		if (client != room_subscription_[pt_cl->id_room].end())
			room_subscription_[static_cast<int>(pt_cl->id_room)].erase(client);

		server_packet::PTCloseRoom pt_close_room(static_cast<uint32_t>(pt_cl->id_room));

		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_close_room);

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
		/*auto subsc = room_subscription_[static_cast<int>(pt_choose_room->id_room)];
		if (auto cli = std::find(subsc.begin(), subsc.end(), static_cast<int>(packet.id_channel)); cli == subsc.end())
			room_subscription_[static_cast<int>(pt_choose_room->id_room)].push_back(static_cast<int>(packet.id_channel));
		*/
			//клиент может быть подписан только на 1 комнату
		for(auto& [id_room, id_channels] : room_subscription_)
		{
			id_channels.erase(std::remove(id_channels.begin(),id_channels.end(),static_cast<int>(packet.id_channel)), id_channels.end());
		}
		room_subscription_[static_cast<int>(pt_choose_room->id_room)].push_back(static_cast<int>(packet.id_channel));
	
		const server_packet::PTInitChooseRoom pt_init_game(pt_choose_room->id_room);
		NetworkServer::getInstance().sendPacket(packet.id_channel, pt_init_game);
		/*

				NetworkServer::getInstance().sendPacket(packet.id_channel, pt_room_state);*/
		break;
	}
	case PacketType::CLI_CHANGE_CONFIG:
	{
		std::cout << "i received PTChangeConfig" << std::endl;
		auto pt_change_config = std::static_pointer_cast<client_packet::PTChangeConfig>(packet.packet);
		rooms_state_.at(static_cast<int>(pt_change_config->id_room)).init(pt_change_config->game_config);
		server_packet::PTNewConfig new_config(pt_change_config->id_room, pt_change_config->game_config);
		NetworkServer::getInstance().sendPacketAllClients(new_config);
		//{//test
		//	std::cout << "new config for room: " << pt_change_config->id_room << std::endl;
		//	std::cout << "eb " << pt_change_config->game_config->energy_base << std::endl;
		//	std::cout << "eac " << pt_change_config->game_config->energy_action_cost << std::endl;
		//	std::cout << "efg " << pt_change_config->game_config->energy_from_grass << std::endl;
		//	std::cout << "etc " << pt_change_config->game_config->energy_to_clone << std::endl;
		//	std::cout << "ut " << pt_change_config->game_config->update_time << std::endl;
		//	std::cout << "gut " << pt_change_config->game_config->grass_update_time << std::endl;
		//}
		break;
	}
	case PacketType::CLI_START_GAME:
	{
		std::cout << "i received PTStartGame" << std::endl;
		auto pt_st = std::static_pointer_cast<client_packet::PTStartGame>(packet.packet);
			//перезапускаем состояние игрового поля
		rooms_state_.at(static_cast<int>(pt_st->id_room)).reset();

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