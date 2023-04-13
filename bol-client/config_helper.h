#pragma once
#include "game_domain.h"
#include <functional>
#include <map>
#include <string>

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

	void doWithAll(const std::function<void(const std::string&, ConfigRecord&)> & visitor);

	const std::map<std::string, ConfigRecord>& getRecords() const { return records; }

	void setOption(/*std::shared_ptr<GameConfig>& config, */const std::string & str, int value)
	{
		records[str].setter_function(value);
	}

private:
	std::map<std::string, ConfigRecord> records;
};
