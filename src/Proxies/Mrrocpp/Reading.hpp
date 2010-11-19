/*
 * Reading.hpp
 *
 *  Created on: 19-11-2010
 *      Author: mateusz
 */

#ifndef READING_HPP_
#define READING_HPP_

namespace Proxies {

namespace Mrrocpp {

class Reading
{
public:
	Reading()
	{
	}

	virtual ~Reading()
	{
	}

	virtual Reading* clone() = 0;
};

}//namespace Mrrocpp
}//namespace Proxies

#endif /* READING_HPP_ */
