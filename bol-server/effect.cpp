#include "pch_server.h"
#include "bacterium.h"
#include "effect.h"

void SpeedEffect::applyEffect(Bacterium& bacterium)
{
	timer_.initInt(5);
	old_speed_ = bacterium.getUpdateTime();
	bacterium.setUpdateTime(bacterium.getUpdateTime() / speed_boost_);
	bacterium.canSpendEnergy(false);
}

void SpeedEffect::detachEffect(Bacterium& bacterium)
{
	bacterium.canSpendEnergy(true);
	bacterium.setUpdateTime(old_speed_);
}

void EnergyEffect::applyEffect(Bacterium& bacterium) 
{
	connection_ = bacterium.onEatAnotherBacterium.connect(boost::bind(&EnergyEffect::boostBactriumEnergy,this,boost::placeholders::_1, boost::placeholders::_2));
}

void EnergyEffect::boostBactriumEnergy(Bacterium& bacterium, int grass_energy) const
{
	bacterium.setEnergy(grass_energy*energy_boost_);
	connection_.disconnect();
}

void CloneEffect::applyEffect(Bacterium& bacterium)
{
	connection_ = bacterium.onUpdate.connect(boost::bind(&CloneEffect::boostBacteriumClone,this, boost::placeholders::_1, boost::placeholders::_2));
	timer_.initInt(3);
}

void CloneEffect::boostBacteriumClone(Bacterium& bacterium, FieldState& state)
{
	if (timer_.timedOut())
	{
		totalClone(bacterium, state);
		connection_.disconnect();
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
		auto child_bacterium = std::make_shared<Bacterium>(bacterium.getIdType(), bacterium.getConfig());
		child_bacterium->setPosition(pos);
		state.addBacterium(child_bacterium);
	}
}