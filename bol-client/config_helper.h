#pragma once
#include <functional>
#include <map>

struct GameConfig
{
	int energy_base = 0;
	int energy_action_cost = 0;
	int energy_to_clone = 0;
	int update_time = 0;
	int grass_update_time = 0;
	int energy_from_grass = 0;
};

class ConfigHelper
{
public:
	struct ConfigRecord
	{
		ConfigRecord() = default;
		ConfigRecord(int col_id, int r_id) : column_id(col_id), row_id(r_id) {}
		int column_id = 0;
		int row_id = 0;
		int default_value = 0;
		std::function<void(int)> setter_function;
	};

	void init(GameConfig& config);

	bool isChanged()
	{
		auto buf = is_change_config_;
		is_change_config_ = false;
		return buf;
	}

	void doWithAll(const std::function<void(const std::string&, ConfigRecord&)> & visitor);

	const std::map<std::string, ConfigRecord>& getRecords() const { return records; }

private:
	std::map<std::string, ConfigRecord> records;
	bool is_change_config_ = false;
	void setOption(GameConfig& config, const std::string & str, int value) { records[str].setter_function(value); }
};
