/*!
 * \file Synchronization.hpp
 * \brief Synchronization policies
 *
 * \author mstefanc
 * \date 06-07-2010
 */

#ifndef SYNCHRONIZATION_HPP_
#define SYNCHRONIZATION_HPP_

#include <boost/thread/mutex.hpp>

namespace Base {

/*!
 * Synchronization policies
 *
 * Every synchronization policy should have form:
 * \code
 * class PolicyName {
 * public:
 *   // enter critical section (lock resource)
 *   void lock();
 *
 *   // exit critical section (release resource)
 *   void unlock();
 * };
 * \endcode
 */
namespace Synchronization {

/*!
 * \brief Synchronization policy - use mutex.
 *
 * Every access is synchronized using internal, per-object mutex.
 */
class Mutex {
public:
	/*!
	 * Lock mutex, wait if it's busy
	 */
	void lock() {
		mtx.lock();
	}

	/*!
	 * Unlock mutex
	 */
	void unlock() {
		mtx.unlock();
	}

private:
	/// Mutex for access synchronization
	boost::mutex mtx;
};

/*!
 * \brief Synchronization policy - no synchronization.
 *
 * Empty methods, no synchronization is performed at all.
 */
class NoSync {
public:
	/*!
	 * Do nothing
	 */
	void lock() {}

	/*!
	 * Do nothing
	 */
	void unlock() {}
};


template<typename T>
class ScopeSync {
public:
	ScopeSync(T & t) : t_(t) {
		t_.lock();
	}

	~ScopeSync() {
		t_.unlock();
	}

private:
	T & t_;
};

} //: Synchronization
} //: Base

#endif /* SYNCHRONIZATION_HPP_ */
