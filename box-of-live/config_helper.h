#pragma once
#include <functional>
#include <map>
#include <memory>

#define ADD_CONFIG_RECORD(option_name, column_id, row_id, default_value)  \
ConfigRecord record_##option_name((column_id), (row_id)); \
record_##option_name.setterFunction = [this](int value) { \
game_config_->option_name = value; \
};  \
record_##option_name.defaultValue = (default_value);\
records[#option_name] = record_##option_name;

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
		int defaultValue = 0;
		std::function<void(int)> setterFunction;
	};

	void init();

	void doWithAll(const std::function<void(const std::string&, ConfigRecord&)> & visitor);

	std::shared_ptr<GameConfig> getGameConfig() const { return game_config_; }

	const std::map<std::string, ConfigRecord>& getRecords() const { return records; }

private:
	std::shared_ptr<GameConfig> game_config_ = std::make_shared<GameConfig>();
	std::map<std::string, ConfigRecord> records;

	void setOption(const std::string & str, int value) { records[str].setterFunction(value); }
};
