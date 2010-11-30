/*
 * ImagePosition.hpp
 *
 *  Created on: 30-11-2010
 *      Author: mboryn
 */

#ifndef IMAGEPOSITION_HPP_
#define IMAGEPOSITION_HPP_

#include <boost/serialization/serialization.hpp>

namespace Types {

struct ImagePosition {
	static const int elementsSize = 4;
	double elements[elementsSize];
private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & elements;
	}
};

} // namespace Types

#endif /* IMAGEPOSITION_HPP_ */
