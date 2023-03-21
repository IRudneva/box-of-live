#pragma once
#include <chrono>

static std::chrono::steady_clock::time_point getCurrentTime() {
	std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
	return time;
}

class Timer {
public:
	void initInt(int interval)
	{
		interval_ = std::chrono::milliseconds(std::chrono::seconds(interval));
		reset();
	}

	void initDouble(double interval)
	{
		int seconds = (int)interval;
		int milli = (int)((interval - seconds) * 1000);
		interval_ = std::chrono::milliseconds(milli);
		reset();
	}

	bool timedOut()
	{
		if (std::chrono::duration_cast<std::chrono::milliseconds>(getCurrentTime() - start_) >= interval_) {
			reset();
			return true;
		}
		return false;
	}

	void reset() { start_ = getCurrentTime(); }

private:
	std::chrono::steady_clock::time_point start_;
	std::chrono::milliseconds interval_ = std::chrono::milliseconds(0);
};