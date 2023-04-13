#pragma once
#include <chrono>
#include <string>
#include <ctime>
#pragma warning(disable : 4996)

static std::chrono::steady_clock::time_point getCurrentTime() {
	std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
	return time;
}

static std::string getCurrentDatetime() {

	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();

	std::time_t t = std::chrono::system_clock::to_time_t(p);

	return std::string(std::ctime(&t), std::strlen(std::ctime(&t)) - 1);
}

class Timer {
public:
	Timer() = default;

	explicit Timer(std::chrono::milliseconds interval) : interval_(interval) { reset(); }

	explicit Timer(int interval) { initInt(interval); }

	explicit Timer(double interval) { initDouble(interval); }

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