#ifndef __ciri_core_timer__
#define __ciri_core_timer__

#include <ciri/core/ITimer.hpp>
#include <Windows.h>

namespace ciri {
	class Timer : public ITimer {
	public:
		Timer();
		virtual ~Timer();

		virtual void start() override;
		virtual void pause() override;
		virtual void stop() override;
		virtual void restart() override;
		virtual double getElapsedMicrosecs() override;
		virtual double getElapsedMillisecs() override;
		virtual double getElapsedSecs() override;

	private:
		LARGE_INTEGER _frequency;
		LARGE_INTEGER _startCount;
		LARGE_INTEGER _endCount;
		bool _isPaused;
	};
} // ciri

#endif /* __ciri_core_timer__ */
