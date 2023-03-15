#pragma once

#include <map>
#include <mutex>
#include <queue>
#include <iostream>

//enum class MsgType : uint32_t
//{
//	MSG_GET_ROOM_LIST
//};
//
//struct Msg
//{
//	MsgType type = MsgType::MSG_GET_ROOM_LIST;
//
//	template<class T>
//	void pack(T& packer) {
//		auto p = static_cast<uint32_t>(type);
//		packer(p);
//	}
//};
//
//struct IdRoom
//{
//	static int generateId() {
//		++id;
//		return id;
//	}
//	static void reset() { id = 0; }
//private:
//	IdRoom() = default;
//	inline static int id = 0;
//};
//
//struct GameRoom
//{
//	GameRoom() { id_room = IdRoom::generateId(); }
//	int field_state = 1;
//	int getIdRoom() const { return id_room; }
//
//	template<class T>
//	void pack(T& packer) {
//		packer(id_room, field_state);
//	}
//
//private:
//	int id_room = 0;
//};
//
//
//enum class PacketType : uint32_t
//{
//	PT_ROOM_LIST
//};
//
//struct Packet
//{
//	PacketType type;
//	std::map<int, GameRoom> room_list;
//
//	template<class T>
//	void pack(T& packer) {
//		auto t = static_cast<uint32_t>(type);
//		packer(t, room_list);
//	}
//
//};

//std::mutex G_MUTEX;
//class SharedMsgQueue
//{
//public:
//	void pushMsg(const Msg m)
//	{
//		std::lock_guard<std::mutex> lock(G_MUTEX);
//		queue_.push(m);
//		std::cout << "Msg: " << (int)m.type << " pushed." << std::endl;
//	}
//
//	Msg popMsg() {
//		std::lock_guard<std::mutex> lock(G_MUTEX);
//		Msg buff_msg = queue_.front();
//		queue_.pop();
//		std::cout << "Msg: " << (int)buff_msg.type << " pop." << std::endl;
//		return buff_msg;
//	}
//
//
//private:
//	std::queue<Msg> queue_;
//};
//
//class SrvManager
//{
//public:
//	SrvManager(){
//		for (int i = 0; i < 3; ++i) {
//			GameRoom room;
//			room_list_.insert({ room.getIdRoom(), room });
//		}
//	}
//	const std::map<int, GameRoom>& getRoomList() { return room_list_; }
//
//	void handleMsg(Msg m)
//	{
//		msg_queue_.pushMsg(m);
//		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//		msg_queue_.popMsg();
//	}
//
//private:
//	std::map<int, GameRoom> room_list_;
//	SharedMsgQueue msg_queue_;
//};