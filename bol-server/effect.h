#pragma once
#include "bacterium.h"
#include "bol_timer.h"
#include <boost/signals2.hpp>

class Bacterium;

class Effect
{
public:
	Effect() = default;

	virtual ~Effect() = default;

	virtual void applyEffect(Bacterium& bacterium) = 0;

	virtual bool isOverAction() = 0;

	virtual void detachEffect(Bacterium& bacterium) = 0;

	//virtual void reset() = 0;
};

class EffectWithTimer : public Effect
{
protected:
	EffectWithTimer() = default;
	Timer timer_;
};

class SpeedEffect : public EffectWithTimer
{
public:
	SpeedEffect() = default;

	void applyEffect(Bacterium& bacterium) override;

	bool isOverAction() override {
		return  timer_.timedOut();
	}

	void detachEffect(Bacterium& bacterium) override;

private:
	const int speed_boost_ = 10;
	int old_speed_ = 0;
};

class EnergyEffect : public Effect
{
public :
	EnergyEffect() = default;

	void applyEffect(Bacterium& bacterium) override;

	bool isOverAction() override { return false; }

	void detachEffect(Bacterium& bacterium) override {}

private:
	const int energy_boost_ = 10;
	boost::signals2::connection connection_;

	void boostBactriumEnergy(Bacterium& bacterium, int grass_energy) const;
	
};

class CloneEffect : public EffectWithTimer
{
public:
	CloneEffect() = default;

	void applyEffect(Bacterium& bacterium) override;

	bool isOverAction() override { return false; }

	void detachEffect(Bacterium& bacterium) override {}

private:
	boost::signals2::connection connection_;

	void boostBacteriumClone(Bacterium& bacterium, FieldState& state);

	void totalClone(Bacterium& bacterium,FieldState& state);
};