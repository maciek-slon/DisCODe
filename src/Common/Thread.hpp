/*!
 * \file Thread.hpp
 * \brief Thread object responsible for handling and executing threads.
 *
 * \author mstefanc
 * \date 2010-05-02
 */

#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Common {

/*!
 * \class Thread
 * \brief Thread object responsible for handling and executing threads.
 *
 * To create new thread new class must inherit Common::Thread and implement run() method.
 * To start new thread with this object simply call start().
 *
 * \author mstefanc
 */
class Thread {
public:

	/*!
	 * Possible thread priorities
	 */
	enum Priority {
		tpRealtime,
		tpHigh,
		tpAbove,
		tpNormal,
		tpBelow,
		tpIdle
	};

	virtual ~Thread() {

	}

	/*!
	 * Begins execution of the thread by calling run() method (which must be implemented in
	 * derived classes).
	 */
	void start() {
		thread = boost::thread(boost::bind( &Thread::run, this ));
	}

	/*!
	 * Wait for thread to complete or the timeout has been reach.
	 * If timeout is less than zero then method never times out.
	 * Method is blocking.
	 * \param timeout timeout in milliseconds to wait for thread to complete its execution
	 */
	void wait(int timeout = -1) {
		if (timeout < 0)
			thread.join();
		else
			thread.timed_join(boost::posix_time::milliseconds(timeout));
	}

	/*!
	 * Gives up the remainder of the current thread's time slice, to allow other threads to run.
	 */
	void yield() {
		boost::this_thread::yield();
	}

	void interrupt() {
		thread.interrupt();
	}

	/*!
	 * Set thread priority
	 * \param priority new thread priority
	 */
	bool setPriority(Priority priority);

	/**
	 * Kill thread
	 */
	bool kill();

	/*!
	 * Suspends the calling thread until the specified time in milliseconds has been reached.
	 */
	static void msleep(int msec) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(msec));
	}

	/*!
	 * Suspends the calling thread until the specified time in microseconds has been reached.
	 */
	static void usleep(int usec) {
		boost::this_thread::sleep(boost::posix_time::microseconds(usec));
	}

protected:
	/*!
	 * This method must be implemented in derived classes and is called when thread starts execution.
	 * Returning from this method will end the execution of the thread.
	 */
	virtual void run() = 0;

	/*!
	 * Suspends the current thread until the specified time in milliseconds has been reached.
	 */
	void sleep(int msec) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(msec));
	}

private:
	/// Actual thread
	boost::thread thread;
};

} //: namespace Common

#endif /* THREAD_HPP_ */
