/*!
 * \file DataStream.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-02-26
 */

#ifndef DATASTREAM_HPP_
#define DATASTREAM_HPP_

#include <string>

namespace Base {

class Connection;

class DataStreamInterface {
public:

	enum dsType {
		dsIn,
		dsOut
	};

    DataStreamInterface(std::string n="name") : name(n) {};

    virtual dsType type() = 0;

    template <class T>
    void set(const T & t) {
    	if (type() == dsIn)
    		internalSet( (void*)(&t) );
    	else
    		throw "Output ports can't receive data!";
    }

protected:
    virtual void internalSet(void * ptr) = 0;

private:
    std::string name;

    Connection * conn;
};

}//: namespace Base

#endif /* DATASTREAM_HPP_ */
