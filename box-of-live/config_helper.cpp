#include "config_helper.h"

void ConfigHelper::init()
{
	ADD_CONFIG_RECORD(energy_base, 0, 0, 5)
	ADD_CONFIG_RECORD(energy_action_cost, 0, 1, 1)
	ADD_CONFIG_RECORD(energy_to_clone, 0, 2, 8)
	ADD_CONFIG_RECORD(update_time, 0, 3, 2)
	ADD_CONFIG_RECORD(grass_update_time, 1, 0, 5)
	ADD_CONFIG_RECORD(energy_from_grass, 1, 1, 3)

	for (const auto&[option_name, config_record] : records)
	{
		setOption(option_name, config_record.defaultValue);
	}
}

void ConfigHelper::doWithAll(const std::function<void(const std::string&, ConfigRecord&)> & visitor)
{
	for (auto&[option_name, config_record] : records) {
		visitor(option_name, config_record);
	}
}