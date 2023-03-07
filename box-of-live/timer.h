#pragma once
#include "domain.h"

class Timer {
public:
	void initInt(int interval);

	void initDouble(double interval);

	bool timedOut();

	void reset() { start_ = getCurrentTime(); }

private:
	std::chrono::steady_clock::time_point start_;
	std::chrono::milliseconds interval_ = std::chrono::milliseconds(0);
};

