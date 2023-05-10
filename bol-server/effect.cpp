#include "pch_server.h"
#include "bacterium.h"
#include "effect.h"
#include <boost/signals2.hpp>

void SpeedEffect::applyEffect(Bacterium& bacterium)
{
	if (is_applied_)
		return;
	timer_.initInt(5);
	old_speed_ = bacterium.getUpdateTime();
	bacterium.setUpdateTime(bacterium.getUpdateTime() / speed_boost_);
	bacterium.canSpendEnergy(false);
	is_applied_ = true;
}

void SpeedEffect::detachEffect(Bacterium& bacterium)
{
	bacterium.canSpendEnergy(true);
	bacterium.setUpdateTime(old_speed_);
	is_applied_ = false;
}

void EnergyEffect::applyEffect(Bacterium& bacterium) 
{
	if (is_applied_)
	{
		logic_is_complete_ = false;
		return;
	}

	using signal_type = boost::signals2::signal<void(Bacterium&, int)>;
	// падает, tracked_obj null
	//bacterium.onEatAnotherBacterium.connect(signal_type::slot_type(&EnergyEffect::logicEffect,this,boost::ref(bacterium), boost::placeholders::_2));
	is_applied_ = true;
}

void EnergyEffect::logicEffect(Bacterium& bacterium, int grass_energy)
{
	if (!is_applied_)
		return;
	if (logic_is_complete_)
		return;

	bacterium.setEnergy(grass_energy*energy_boost_);
	logic_is_complete_ = true;
}

void CloneEffect::applyEffect(Bacterium& bacterium)
{
	if (is_applied_)
		return;
	// падает, tracked_obj null
	//bacterium.onUpdate.connect(signal_type::slot_type(&CloneEffect::logicEffect,this,boost::ref(bacterium), boost::placeholders::_2));
	timer_.initInt(3);
	is_applied_ = true;
}

void CloneEffect::logicEffect(Bacterium& bacterium, FieldState& state)
{
	if (!is_applied_)
		return;
	if (logic_is_complete_)
		return;

	if (timer_.timedOut())
	{
		totalClone(bacterium, state);
		logic_is_complete_ = true;
	}
}

void CloneEffect::totalClone(Bacterium& bacterium, FieldState& state) 
{
	auto bac_pos = bacterium.getPosition();

	auto adj = state.getPositionsAround(bac_pos);
	
	for (auto&[pos, c] : adj) {
		if (pos == bac_pos)
			continue;
		if (c == nullptr)
			continue;
		state.resetTypeCell(c->getIdCell());
	}

	for (auto&[pos, cell] : adj) {
		auto child_bacterium = std::make_shared<Bacterium>(bacterium.getIdType(), std::make_shared<GameConfig>(bacterium.getConfig()));
		child_bacterium->setPosition(pos);
		state.addBacterium(child_bacterium);
	}
}