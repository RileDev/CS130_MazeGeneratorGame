#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

class CountdownTimer {
private:
	int minutes;
	int seconds;
	bool isRunning = false;
	bool isOver = false;
	chrono::steady_clock::time_point lastTick;

	void updateTimer() {
		if (seconds > 0)
			seconds--;
		else if (minutes > 0) {
			minutes--;
			seconds = 59;
		}
	}

public:
	CountdownTimer(int m = 0, int s = 0) : minutes(m), seconds(s) {
		minutes += seconds / 60;
		seconds %= 60;
	}

	void start() {
		isOver = (minutes == 0 && seconds == 0);
		isRunning = !isOver;
		lastTick = std::chrono::steady_clock::now();
	}

	void update() {
		if (!isRunning || isOver) return;

		auto now = std::chrono::steady_clock::now();
		using sec = std::chrono::seconds;

		if (now - lastTick >= sec(1)) {
			// catch up if more than 1s passed
			int ticks = std::chrono::duration_cast<sec>(now - lastTick).count();
			lastTick += sec(ticks);
			while (ticks-- > 0 && (minutes > 0 || seconds > 0)) {
				updateTimer();
			}
			if (minutes == 0 && seconds == 0) {
				isOver = true;
				isRunning = false;
			}
		}
	}

	void stop() { isRunning = false; }

	bool over() const { return isOver; }
	bool running() const { return isRunning; }

	string getFormatTime() const {
		ostringstream oss;
		oss << std::setfill('0') << std::setw(2) << minutes
			<< ":"
			<< std::setfill('0') << std::setw(2) << seconds;
		return oss.str();
	}
};