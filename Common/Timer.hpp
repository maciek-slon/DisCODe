/*!
 * \file Timer.hpp
 * \brief Simple timer object
 *
 * \author mstefanc
 * \date 2010-05-07
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <ctime>

namespace Common {

/*!
 * \brief Simple timer object that measures elapsed time.
 */
class Timer {
public:
	/*!
	 * Default constructor.
	 *
	 * \post elapsed() == 0
	 */
	Timer() {
		start_time = std::clock();
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
	double elapsed() const {
		return double(std::clock() - start_time) / CLOCKS_PER_SEC;
	}

private:
	/// Start time
	std::clock_t start_time;
};

} //: namespace Common

#endif /* TIMER_HPP_ */
