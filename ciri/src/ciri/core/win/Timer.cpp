#include <ciri/core/win/Timer.hpp>
#include <memory>

using namespace ciri::core;

	Timer::Timer()
		: ITimer() {
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
	
	double Timer::getElapsedMicrosecs() {
		if( !_isPaused ) {
			QueryPerformanceCounter(&_endCount);
		}
		const double startTime = _startCount.QuadPart * (1000000.0 / _frequency.QuadPart);
		const double endTime = _endCount.QuadPart * (1000000.0 / _frequency.QuadPart);
		return (endTime - startTime);
	}
	
	double Timer::getElapsedMillisecs() {
		return getElapsedMicrosecs() * 0.001;
	}
	
	double Timer::getElapsedSecs() {
		return getElapsedMicrosecs() * 0.000001;
	}

	namespace ciri { namespace core {
	std::shared_ptr<ITimer> createTimer() {
		return std::shared_ptr<ITimer>(new Timer());
	}
	}}