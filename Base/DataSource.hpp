/*!
 * \file DataSource.hpp
 * \brief Abstract interface for all data sources.
 *
 * \author tkornuta
 * \date 2009-11-05
 */

#ifndef DATASOURCE_HPP_
#define DATASOURCE_HPP_

#include <iostream>

namespace Base {

/*!
 * \class DataSource
 * \brief Abstract interface class for all data sources.
 * Goal of every data source is to retrieve data from "device".
 * \author tkornuta
 * \date 2009-11-05
 */
class DataSource
{
public:
	/*!
	 * Base constructor
	 */
	DataSource()
	{
		std::cout << "Abstract DataSource: Hello\n";
	}

	/*!
	 * Virtual destructor - empty.
	 */
	virtual ~DataSource()
	{
		std::cout << "Abstract DataSource: Bye \n";
	}

	/*!
	 * Connects source to given device.
	 */
	virtual void connect() = 0;

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	virtual void disconnect() = 0;

	/*!
	 * Retrieves data from device.
	 */
	virtual void* retrieveData() = 0;

};

}//: namespace Base


#endif /* DATASOURCE_HPP_ */
