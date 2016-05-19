#ifndef __ciri_core_ITimer__
#define __ciri_core_ITimer__

namespace ciri {

class ITimer {
public:
	ITimer() {
	}

	virtual ~ITimer() {
	}

		
	/**
		* Starts the timer.
		*/
	virtual void start()=0;

	/**
		* Pauses the timer.
		*/
	virtual void pause()=0;

	/**
		* Stops the timer.
		*/
	virtual void stop()=0;

	/**
		* Restarts the timer.
		*/
	virtual void restart()=0;

	/**
		* Gets the elapsed time since start in microseconds.
		* @return Time elapsed in microseconds.
		*/
	virtual double getElapsedMicrosecs()=0;

	/**
		* Gets the elapsed time since start in milliseconds.
		* @return Time elapsed in milliseconds.
		*/
	virtual double getElapsedMillisecs()=0;

	/**
		* Gets the elapsed time since start in seconds.
		* @return Time elapsed in seconds.
		*/
	virtual double getElapsedSecs()=0;
};

}

#endif