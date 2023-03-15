#pragma once
#include <map>
#include <mutex>
#include <queue>
#include <iostream>

enum class MsgType
{
	MSG_GET_ROOM_LIST
};

struct Msg
{
	MsgType type;
};

std::mutex G_MUTEX;
class SharedMsgQueue
{
public:
	void pushMsg(const Msg m)
	{
		std::lock_guard<std::mutex> lock(G_MUTEX);
		queue_.push(m);
		std::cout << "Msg: " << (int)m.type << " pushed." << std::endl;
	}

	Msg popMsg() {
		std::lock_guard<std::mutex> lock(G_MUTEX);
		Msg buff_msg = queue_.front();
		queue_.pop();
		std::cout << "Msg: " << (int)buff_msg.type << " pop." << std::endl;
		return buff_msg;
	}


private:
	std::queue<Msg> queue_;
};

struct IdRoom
{
	static int generateId() {
		++id;
		return id;
	}
	static void reset() { id = 0; }
private:
	IdRoom() = default;
	inline static int id = 0;
};

struct GameRoom
{
	GameRoom() { id_room = IdRoom::generateId(); }
	int field_state = 1;
	 int getIdRoom() const { return id_room; }
private:
	int id_room = 0;
};

class SrvManager
{
public:
	const std::map<int, GameRoom>& getRoomList() { return room_list_; }
	
	void handleMsg(Msg m)
	{
		msg_queue_.pushMsg(m);
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		msg_queue_.popMsg();
	}

private:
	std::map<int, GameRoom> room_list_;
	SharedMsgQueue msg_queue_;
};