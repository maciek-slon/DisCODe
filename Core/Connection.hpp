/*!
 * \file Connection.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-02-26
 */

#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <string>
#include <vector>

#include <boost/foreach.hpp>

#include "DataStreamInterface.hpp"

namespace Core {

/*!
 * \class Connection
 * \brief
 * \author mstefanc
 */
class Connection {
public:
	Connection(std::string n="unnamed_connection") : name(n) {};

	template <class T>
	void send(const T & t);

protected:

private:
	std::string name;

	std::vector<DataStreamInterface *> listeners;
};

template <class T>
void Connection::send(const T & t) {
	BOOST_FOREACH( DataStreamInterface * i, listeners ) {
		i->set(t);
	}
}

} //: namespace Core

#endif /* CONNECTION_HPP_ */
