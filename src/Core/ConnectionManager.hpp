/*!
 * \file ConnectionManager.hpp
 * \brief DataStream connections management
 *
 * \author mstefanc
 * \date 2010-02-28
 */

#ifndef CONNECTIONMANAGER_HPP_
#define CONNECTIONMANAGER_HPP_

#include <string>
#include <map>

#include <boost/foreach.hpp>

#include "Connection.hpp"

namespace Core {

/*!
 * \class ConnectionManager
 * \brief DataStream connections management class.
 *
 * DataStreams are connected through \ref Base::Connection "connections". If connection
 * already exists new DataStreams are simply added to existing object, otherwise new connection is created.
 *
 * \author mstefanc
 */
class ConnectionManager {

public:
	/*!
	 * Destructor, called only once, when related singleton is destroyed.
	 */
	~ConnectionManager() {
	}
	
	void release() {
		BOOST_FOREACH(con_pair con, connections)
			delete con.second;
	}

	/*!
	 * Returns connection with given name. If there's no such connection
	 * then new one is created.
	 */
	Base::Connection * get(const std::string & name) {
		if (connections.count(name) == 0) {
			Base::Connection * con = new Base::Connection(name);
			connections[name] = con;
		}

		return connections[name];
	}

	/*!
	 *
	 */
	ConnectionManager() {

	}

protected:
	/*!
	 * List of already created connections, name of connection is key value.
	 */
	std::map<std::string, Base::Connection *> connections;
	
	typedef std::pair<std::string, Base::Connection *> con_pair;
};

} //: namespace Core

#endif /* CONNECTIONMANAGER_HPP_ */
