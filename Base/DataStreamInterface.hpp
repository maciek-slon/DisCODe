/*!
 * \file DataStream.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-02-26
 */

#ifndef DATASTREAMINTERFACE_HPP_
#define DATASTREAMINTERFACE_HPP_

#include <string>

namespace Base {

class Connection;

/*!
 * \class DataStreamInterface
 */
class DataStreamInterface {
public:

	enum dsType {
		dsIn,
		dsOut
	};

    DataStreamInterface(std::string n="name") : name(n) {};

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

protected:
    virtual void internalSet(void * ptr) = 0;

    Connection * conn;

private:
    std::string name;

};

}//: namespace Base

#endif /* DATASTREAMINTERFACE_HPP_ */
