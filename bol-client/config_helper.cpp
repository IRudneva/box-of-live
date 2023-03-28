#include "pch_client.h"
#include "config_helper.h"

void ConfigHelper::init(GameConfig& config)
{
#define ADD_CONFIG_RECORD(option_name, column_id, row_id, def_value)  \
ConfigRecord record_##option_name((column_id), (row_id)); \
record_##option_name.setter_function = [&config, this](int value) { \
config.option_name = value; \
is_change_config_ = true;\
};  \
record_##option_name.default_value = (def_value);\
records[#option_name] = record_##option_name;


	ADD_CONFIG_RECORD(energy_base, 0, 0, 5)
	ADD_CONFIG_RECORD(energy_action_cost, 0, 1, 1)
	ADD_CONFIG_RECORD(energy_to_clone, 0, 2, 8)
	ADD_CONFIG_RECORD(update_time, 0, 3, 2)
	ADD_CONFIG_RECORD(grass_update_time, 0, 4, 5)
	ADD_CONFIG_RECORD(energy_from_grass, 0, 5, 3)
#undef ADD_CONFIG_RECORD

	for (const auto&[option_name, config_record] : records)
	{
		setOption(config, option_name, config_record.default_value);
	}
}

void ConfigHelper::doWithAll(const std::function<void(const std::string&, ConfigRecord&)> & visitor)
{
	for (auto&[option_name, config_record] : records) {
		visitor(option_name, config_record);
	}
}