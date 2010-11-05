/*!
 * \file DataStream.hpp
 * \brief Concrete implementations of data streams
 */

#ifndef DATASTREAM_HPP_
#define DATASTREAM_HPP_

#include "DataStreamInterface.hpp"
#include "Connection.hpp"

#include "Policies/DataStreamBuffer.hpp"
#include "Policies/Synchronization.hpp"


#include <boost/shared_ptr.hpp>

namespace Base {


/*!
 * \brief Input data stream.
 *
 * \tparam T type of data to be handled by DataStream
 * \tparam BufferingPolicy buffering policy (way to store data)
 * \tparam ReadSync synchronization of read access
 * \tparam WriteSync synchronization of write access
 */
template
<
    typename T,
    template <class T> class BufferingPolicy = DataStreamBuffer::Queue,
    class Sync = Synchronization::NoSync
>
class DataStreamIn : public DataStreamInterface, public BufferingPolicy<T>
{
	using BufferingPolicy<T>::retrieve;

	/// Object used for synchronization of data reading
	Sync sync;

public:
	virtual dsType type() {
		return dsIn;
	}

	T read() {
		Synchronization::ScopeSync<Sync> ss(sync);
		//sync.lock();
		T t = retrieve();
		//sync.unlock();
		return t;
	}

protected:
	virtual void internalSet(void * ptr) {
		Synchronization::ScopeSync<Sync> ss(sync);
		//sync.lock();
		T t = *((T*)ptr);
		store(t);
		//sync.unlock();
	}
};


/*!
 * \class DataStreamOut
 * \brief Output data stream
 */
template <class T>
class DataStreamOut : public DataStreamInterface {
public:
	DataStreamOut(std::string n = "name") : DataStreamInterface(n) {}

	virtual dsType type() {
		return dsOut;
	}

	void write (const T & t) {
		if (conn)
			conn->send(t);
	}

protected:
	virtual void internalSet(void * ptr) {
		// intentionally left empty
	}

private:
};

/*!
 * \brief Input data stream, retrieve pointer to stored data.
 *
 * It's usefull when one have to receive pointer to base class, when objects of
 * derived class are sent.
 *
 * \tparam T type of data to be handled by DataStream
 * \tparam BufferingPolicy buffering policy (way to store data)
 * \tparam ReadSync synchronization of read access
 * \tparam WriteSync synchronization of write access
 */
template
<
    typename T,
    template <class T> class BufferingPolicy = DataStreamBuffer::Queue,
    class Sync = Synchronization::NoSync
>
class DataStreamInPtr : public DataStreamInterface, public BufferingPolicy< boost::shared_ptr<T> >
{
	using BufferingPolicy< boost::shared_ptr<T> >::retrieve;

	/// Object used for synchronization of data reading
	Sync sync;

public:
	virtual dsType type() {
		return dsIn;
	}

	boost::shared_ptr<T> read() {
		Synchronization::ScopeSync<Sync> ss(sync);
		//sync.lock();
		boost::shared_ptr<T> t = retrieve();
		//sync.unlock();
		return t;
	}

protected:
	virtual void internalSet(void * ptr) {
		Synchronization::ScopeSync<Sync> ss(sync);
		//sync.lock();
		T* t = (T*)ptr;
		boost::shared_ptr<T> p(t->clone());
		store(p);
		//sync.unlock();
	}
};


}//: namespace Base

#endif /* DATASTREAM_HPP_ */
