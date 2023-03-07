#include "timer.h"

void Timer::initInt(int interval) {
	interval_ = std::chrono::milliseconds(std::chrono::seconds(interval));
	reset();
}

void Timer::initDouble(double interval) {
	int seconds = (int)interval;
	int milli = (int)((interval - seconds) * 1000);
	interval_ = std::chrono::milliseconds(milli);
	reset();
}

bool Timer::timedOut() {
	if (std::chrono::duration_cast<std::chrono::milliseconds>(getCurrentTime() - start_) >= interval_) {
		reset();
		return true;
	}
	return false;
}