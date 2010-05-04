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

namespace Base {

/*!
 * \class DataStreamIn
 * \brief Input data stream
 */
template <class T>
class DataStreamIn : public DataStreamInterface {
public:
	DataStreamIn(std::string n = "name") : DataStreamInterface(n) {};

	virtual dsType type() {
		return dsIn;
	}

	T read() {
		if (buffer.size()>0) {
			T t = buffer.front();
			buffer.pop_front();
			return t;
		} else {
			throw "Buffer empty!";
		}
	}

protected:
	virtual void internalSet(void * ptr) {
		T t = *((T*)ptr);
		buffer.push_back(t);
	}

private:
	std::list<T> buffer;
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
