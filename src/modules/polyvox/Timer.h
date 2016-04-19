#pragma once

#include <chrono>
#include <cstdint>

namespace PolyVox {

class Timer {
public:
	Timer(bool bAutoStart = true) {
		if (bAutoStart) {
			start();
		}
	}

	void start() {
		m_start = clock::now();
	}

	float elapsedTimeInSeconds() {
		std::chrono::duration<float> elapsed_seconds = clock::now() - m_start;
		return elapsed_seconds.count();
	}

	float elapsedTimeInMilliSeconds() {
		std::chrono::duration<float, std::milli> elapsed_milliseconds = clock::now() - m_start;
		return elapsed_milliseconds.count();
	}

	float elapsedTimeInMicroSeconds() {
		std::chrono::duration<float, std::micro> elapsed_microseconds = clock::now() - m_start;
		return elapsed_microseconds.count();
	}

private:
	typedef std::chrono::system_clock clock;
	std::chrono::time_point<clock> m_start;
};

}
