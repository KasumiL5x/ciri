#ifndef __ciri_timer__
#define __ciri_timer__

#include <Windows.h>

namespace ciri {
	class Timer {
	public:
		Timer();
		~Timer();

		void start();
		void pause();
		void stop();
		void restart();

		double getElapsedMicroseconds();
		double getElapsedMilliseconds();
		double getElapsedSeconds();

	private:
		LARGE_INTEGER _frequency;
		LARGE_INTEGER _startCount;
		LARGE_INTEGER _endCount;
		bool _isPaused;
	};
} // ciri

#endif /* __ciri_timer__ */
