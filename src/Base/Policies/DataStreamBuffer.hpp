/*!
 * \file DataStreamBuffer.hpp
 * \brief DataStream buffering policies
 *
 * \author mstefanc
 * \date 06-07-2010
 */

#ifndef DATASTREAMBUFFER_HPP_
#define DATASTREAMBUFFER_HPP_

#include <queue>

namespace Base {

/*!
 * Buffering policies for DataStream.
 *
 * Every buffering policy should have form:
 * \code
 * template <class T>
 * class PolicyName {
 * protected:
 *   // store data in some internal container
 *   void store(const T & t);
 *
 *   // retrieve data from internal container
 *   T retrieve();
 * };
 * \endcode
 *
 * There are some predefined simple policies (like FIFO queue, keeping only newest or oldest data).
 */
namespace DataStreamBuffer {

/*!
 * \brief Buffering policy - FIFO queue.
 *
 * All items are stored in queue, so that none of them is missed during processing.
 */
template <class T>
class Queue {
public:
	bool empty() const {
		return buffer.empty();
	}

protected:
	/*!
	 * Push data on the end of queue
	 * \param t data to be stored
	 */
	void store(const T & t) {
		buffer.push(t);
	}

	/*!
	 * Get first element from queue
	 * \return First element from queue
	 * \throw exception queue is empty
	 */
	T retrieve() {
		if (buffer.size() > 0) {
			T t = buffer.front();
			buffer.pop();
			return t;
		} else {
			/// \todo Throw correct exception
			throw "Empty buffer!";
			// throwing disabled until scopeLock will be implemented in DataStream
		}
	}

private:
	//// FIFO queue
	std::queue<T> buffer;
};

/*!
 * \brief Buffering policy - keep only the newest data.
 *
 * Only the newest data is remembered, when data comes before reading previous one,
 * older item is lost.
 */
template <class T>
class Newest {
public:
	/*!
	 * Clears internal flags state
	 */
	Newest() {
		fresh = false;
	}

	bool empty() const {
		return !fresh;
	}

protected:
	/*!
	 * Remember new data.
	 * \param t data to be remembered
	 */
	void store(const T & t) {
		item = t;
		fresh = true;
	}

	/*!
	 * Return remembered data (if it's fresh).
	 *
	 * If there is no new data since last read, exception is thrown.
	 *
	 * \return remembered data
	 * \throw exception there is no fresh data since last read
	 */
	T retrieve() {
		if (fresh) {
			fresh = false;
			return item;
		} else {
			/// \todo Throw correct exception
			throw "Fresh data not available!";
		}
	}

private:
	/// newest data
	T item;

	/// data freshness flag
	bool fresh;
};

/*!
 * \brief Buffering policy - keep only the oldest data
 *
 * Only the oldest data is remembered, when data comes before reading previous one,
 * newer item is lost.
 */
template <class T>
class Oldest {
public:
	/*!
	 * Clears internal flags state
	 */
	Oldest() {
		fresh = false;
	}

protected:
	/*!
	 * Remember new data
	 *
	 * If there is older, unread data then new one is lost.
	 *
	 * \param t data to be remembered
	 */
	void store(const T & t) {
		if (!fresh) {
			item = t;
			fresh = true;
		}
	}

	/*!
	 * Return remembered data (if it's fresh).
	 *
	 * If there is no data since last read, exception is thrown.
	 *
	 * \return remembered data
	 * \throw exception there is no fresh data since last read
	 */
	T retrieve() {
		if (fresh) {
			fresh = false;
			return item;
		} else {
			/// \todo Throw correct exception
			throw "No data available!";
		}
	}

private:
	/// oldest data
	T item;

	/// data freshness flag
	bool fresh;
};

} //: DataStreamBuffer
} //: Base

#endif /* DATASTREAMBUFFER_HPP_ */
