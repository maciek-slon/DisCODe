/*!
 * \file Timer.hpp
 * \brief Simple timer object
 *
 * \author mstefanc
 * \date 2010-05-07
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#if defined(UNIFIED_TIMER)
#	include <ctime>
#elif defined(WIN32)
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#else
#	include <ctime>
#endif

namespace Common {

/*!
 * \brief Simple timer object that measures elapsed time.
 */
class Timer {
public:
#if defined(UNIFIED_TIMER)
	/*!
	 * Default constructor.
	 *
	 * \post elapsed() == 0
	 */
	Timer() {
		start_time = std::clock();
		freq = CLOCKS_PER_SEC;
	}

	/*!
	 * Restart timer
	 *
	 * \post elapsed() == 0
	 */
	void restart() {
		start_time = std::clock();
	}

	/*!
	 * Return elapsed time in seconds
	 */
	double elapsed() {
		return double(std::clock() - start_time) / freq;
	}

	long frequency() const {
		return freq;
	}
#elif defined(WIN32)
	/*!
	 * Default constructor.a
	 *
	 * \post elapsed() == 0
	 */
	Timer() {
		QueryPerformanceFrequency(&freq);
	}

	/*!
	 * Restart timer
	 *
	 * \post elapsed() == 0
	 */
	void restart() {
		QueryPerformanceCounter(&start_time);
	}

	/*!
	 * Return elapsed time in seconds
	 */
	double elapsed() {
		QueryPerformanceCounter(&end_time);
		return double(end_time.QuadPart - start_time.QuadPart) / freq.QuadPart;
	}

	long frequency() const {
		return freq.QuadPart;
	}
#else
	/*!
	 * Default constructor.
	 *
	 * \post elapsed() == 0
	 */
	Timer() {
		clock_getres(CLOCK_REALTIME, &start_time);
		freq = (long)(1.0 / (start_time.tv_sec + 0.001*0.001*0.001*start_time.tv_nsec));
	}

	/*!
	 * Restart timer
	 *
	 * \post elapsed() == 0
	 */
	void restart() {
		clock_gettime(CLOCK_REALTIME, &start_time);
	}

	/*!
	 * Return elapsed time in seconds
	 */
	double elapsed() {
		clock_gettime(CLOCK_REALTIME, &end_time);
		double ns = 0.001*0.001*0.001*(end_time.tv_nsec - start_time.tv_nsec);
		return double(end_time.tv_sec - start_time.tv_sec + ns);
	}

	long frequency() const {
		return freq;
	}
#endif /* WIN32 */

private:
#if defined (UNIFIED_TIMER)
	/// ticks per second
	long freq;
	/// start time
	clock_t start_time;
	/// end time
	clock_t end_time;
#elif defined(WIN32)
	/// ticks per second
    LARGE_INTEGER freq;
    /// start time
    LARGE_INTEGER start_time;
    /// end time
    LARGE_INTEGER end_time;
#else /* *nix */
    /// ticks per second
    long freq;
    /// start time
	timespec start_time;
	/// end time
	timespec end_time;
#endif /* WIN32 */
};

} //: namespace Common

#endif /* TIMER_HPP_ */
