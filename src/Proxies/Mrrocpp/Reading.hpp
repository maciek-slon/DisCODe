/*
 * Reading.hpp
 *
 */

#ifndef READING_HPP_
#define READING_HPP_

#include "xdr/xdr_oarchive.hpp"

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
private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		// serialize base class information
		ar & boost::serialization::base_object <bus_stop>(*this);
		ar & street1;
		ar & street2;
	}

};

}//namespace Mrrocpp
}//namespace Proxies

#endif /* READING_HPP_ */
