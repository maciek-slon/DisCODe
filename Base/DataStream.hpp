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

namespace Base {

template <class T>
class DataStreamIn : public DataStreamInterface {
	DataStreamIn(std::string n = "name") : DataStreamInterface(n) {};

	void write (const T & t) {
		conn->send(t);
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

}//: namespace Base

#endif /* DATASTREAM_HPP_ */
