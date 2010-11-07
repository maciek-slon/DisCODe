/*!
 * \file DataStreamInterface.hpp
 * \brief Interface class for DataStreams.
 */

#ifndef DATASTREAMINTERFACE_HPP_
#define DATASTREAMINTERFACE_HPP_

#include <string>

namespace Base {

class Connection;

/*!
 * \class DataStreamInterface
 * \brief Interface class for DataStreams.
 *
 * Provides base functionalities for all data streams, and is used to keep
 * track of already defined streams (pointers to data streams are hard to
 * manage as DataStream is template class).
 */
class DataStreamInterface {
public:

	enum dsType {
		dsIn,
		dsOut
	};

    DataStreamInterface(const std::string n="name") : conn(NULL), name_(n) {};

    virtual ~DataStreamInterface() {

    }

    virtual dsType type() = 0;

    template <class T>
    void set(const T & t) {
    	if (type() == dsIn)
    		internalSet( (void*)(&t) );
    	else
    		throw "Output ports can't receive data!";
    }


	void setConnection(Connection * con) {
		conn = con;
	}

	const std::string & name() const {
		return name_;
	}

protected:
    virtual void internalSet(void * ptr) = 0;

    Connection * conn;

private:
    const std::string name_;

};

}//: namespace Base

#endif /* DATASTREAMINTERFACE_HPP_ */
