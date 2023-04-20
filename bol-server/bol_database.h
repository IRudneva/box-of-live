#pragma once
#include <mutex>
#include <queue>
#include "data_for_save.h"

using namespace  sqlite;

class DataBuffer
{
public:
	static DataBuffer* getInstance()
	{
		static DataBuffer inst;
		return &inst;
	}

	void pushData(std::shared_ptr<DataForSave> data)
	{
		std::lock_guard<std::mutex> lock(m_);
		data_.push_back(data);
	}

	bool hasData()
	{
		std::lock_guard<std::mutex> lock(m_);
		if (data_.empty())
			return false;

		return true;
	}

	std::vector<std::shared_ptr<DataForSave>> popData()
	{
		std::lock_guard<std::mutex> lock(m_);
		std::vector<std::shared_ptr<DataForSave>> buffer = data_;
		data_.clear();
		return buffer;
	}

private:
	std::mutex m_;
	std::vector<std::shared_ptr<DataForSave>> data_;

	DataBuffer(const DataBuffer&) = delete;
	DataBuffer& operator=(DataBuffer&) = delete;
	DataBuffer() = default;
	~DataBuffer() = default;
};


class DatabaseHandler
{
public:
	explicit DatabaseHandler(database db) : db_(std::move(db))
	{
		try
		{
			createTable();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	void saveData(std::vector<std::shared_ptr<DataForSave>> data_for_save)
	{
		clearData();

		for(auto& data : data_for_save)
		{
			auto raw_data = std::static_pointer_cast<DataForTabRoomsState>(data);
			try
			{
				db_ << "INSERT INTO rooms_state (id_room, pos_x, pos_y, type_cell) VALUES (?,?,?,?)"
					<< raw_data->id_room
					<< raw_data->x << raw_data->y
					<< raw_data->cell_type;
			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}
	}

	std::vector<int> getAllRooms(const std::string& request)
	{
		std::vector<int> res;
		db_ << request >> [&](int id) {res.push_back(id); };
		return res;
	}

	std::vector<DataForTabRoomsState> getAllStates(const std::string& request)
	{
		std::vector<DataForTabRoomsState> res;
		db_ << request >> [&](int id, int x, int y, int cell_type) {res.emplace_back(id, x, y, static_cast<TypeCell>(cell_type)); };
		return res;
	}


private:
	database db_;

	void createTable()
	{
		db_ << "CREATE TABLE IF NOT EXISTS rooms_state ("
			"id_room INTEGER,"	
			"pos_x INTEGER,"
			"pos_y INTEGER,"
			"type_cell INTEGER"
			");";
	}

	void clearData()
	{
		try
		{
			db_ << "TRUNCATE TABLE rooms_state;";
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
};


class QueueRequest
{
public:
	static QueueRequest* getInstance()
	{
		static QueueRequest inst;
		return &inst;
	}

	void init(std::shared_ptr<DatabaseHandler> dbh)
	{
		handler_ = dbh;
	}

	std::vector<DataForTabRoomsState> GetStates(const std::string& request)
	{
		std::lock_guard<std::mutex> lock(m_);
		return handler_->getAllStates(request);
	}

	std::vector<int> GetAllRooms(const std::string& request)
	{
		std::lock_guard<std::mutex> lock(m_);
		return handler_->getAllRooms(request);
	}


	//bool hasRequest()
	//{
	//	std::lock_guard<std::mutex> lock(m_);
	//	if (queue_.empty())
	//		return false;

	//	return true;
	//}

	//std::string popRequest()
	//{
	//	std::lock_guard<std::mutex> lock(m_);
	//	auto buffer = queue_.front();
	//	queue_.pop();
	//	return buffer;
	//}

private:
	std::mutex m_;
	//std::queue<std::string> queue_;
	std::shared_ptr<DatabaseHandler> handler_;

	QueueRequest(const QueueRequest&) = delete;
	QueueRequest& operator=(QueueRequest&) = delete;
	QueueRequest() = default;
	~QueueRequest() = default;
};