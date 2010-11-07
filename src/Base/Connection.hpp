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
#include "Logger.hpp"

namespace Base {

/*!
 * \class Connection
 * \brief Connection between \ref DataStreamInterface "data streams".
 *
 * Each connection keeps information about connected receivers, and when
 * any of \ref DataStreamOut "output streams" sends data through this connection
 * it's delivered to all connected streams.
 *
 * \author mstefanc
 */
class Connection {
public:
	Connection(std::string n="unnamed_connection") : name(n) {};

	template <class T>
	void send(const T & t) {
		BOOST_FOREACH( DataStreamInterface * i, listeners )
			i->set(t);
	}

	void addListener(DataStreamInterface * listener) {
		LOG(LINFO) << "Connection " << name << ": added new listener: " << listener->name();
		listeners.push_back(listener);
	}

protected:

private:
	const std::string name;

	std::vector<DataStreamInterface *> listeners;
};

} //: namespace Base

#endif /* CONNECTION_HPP_ */
