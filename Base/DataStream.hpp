/*!
 * \file DataStream.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-02-26
 */

#ifndef DATASTREAM_HPP_
#define DATASTREAM_HPP_

#include "DataStreamInterface.hpp"
#include "Connection.hpp"

#include "Policies/DataStreamBuffer.hpp"
#include "Policies/Synchronization.hpp"

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
    class ReadSync = Synchronization::Mutex,
    class WriteSync = Synchronization::Mutex
>
class DataStreamIn : public DataStreamInterface, public BufferingPolicy<T>
{
	using BufferingPolicy<T>::retrieve;

	/// Object used for synchronization of data reading
	ReadSync read_sync;

	/// Object used for synchronization of data writing
	WriteSync write_sync;

public:
	virtual dsType type() {
		return dsIn;
	}

	T read() {
		read_sync.lock();
		T t = retrieve();
		read_sync.unlock();
		return t;
	}

protected:
	virtual void internalSet(void * ptr) {
		write_sync.lock();
		T t = *((T*)ptr);
		store(t);
		write_sync.unlock();
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

}//: namespace Base

#endif /* DATASTREAM_HPP_ */
