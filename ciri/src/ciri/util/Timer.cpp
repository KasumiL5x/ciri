#include <ciri/util/Timer.hpp>

namespace ciri {
	Timer::Timer() {
		stop();
	}

	Timer::~Timer() {
	}

	void Timer::start() {
		_isPaused = false;
		QueryPerformanceCounter(&_startCount);
	}

	void Timer::pause() {
		_isPaused = true;
		QueryPerformanceCounter(&_endCount);
	}

	void Timer::stop() {
		QueryPerformanceFrequency(&_frequency);
		_startCount.QuadPart = 0;
		_endCount.QuadPart = 0;
		_isPaused = true;
	}

	void Timer::restart() {
		stop();
		start();
	}

	double Timer::getElapsedMicroseconds() {
		if( !_isPaused ) {
			QueryPerformanceCounter(&_endCount);
		}
		const double startTime = _startCount.QuadPart * (1000000.0 / _frequency.QuadPart);
		const double endTime = _endCount.QuadPart * (1000000.0 / _frequency.QuadPart);
		return (endTime - startTime);
	}

	double Timer::getElapsedMilliseconds() {
		return getElapsedMicroseconds() * 0.001;
	}

	double Timer::getElapsedSeconds() {
		return getElapsedMicroseconds() * 0.000001;
	}
} // ciri