#pragma once
#include "bacterium.h"
#include "bacterium.h"
#include "bol_timer.h"

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
		return  (is_applied_ ? timer_.timedOut() : false);
	}

	void detachEffect(Bacterium& bacterium) override;
/*
	void reset() override
	{
		Timer timer;
		timer_ = std::move(timer);
		is_applied_ = false;
		old_speed_ = 0;
	}
*/
private:
	const int speed_boost_ = 10;
	int old_speed_ = 0;
	bool is_applied_ = false;
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
	bool is_applied_ = false;
	bool logic_is_complete_ = false;

	void logicEffect(Bacterium& bacterium, int grass_energy);
	
};

class CloneEffect : public EffectWithTimer
{
public:
	CloneEffect() = default;

	void applyEffect(Bacterium& bacterium) override;

	bool isOverAction() override { return false; }

	void detachEffect(Bacterium& bacterium) override {}

private:
	bool is_applied_ = false;
	bool logic_is_complete_;

	void logicEffect(Bacterium& bacterium, FieldState& state);

	void totalClone(Bacterium& bacterium,FieldState& state);
};