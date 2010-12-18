/*
 * PBReading.hpp
 *
 *  Created on: 19-11-2010
 *      Author: mateusz
 */

#ifndef IBREADING_HPP_
#define IBREADING_HPP_

#include "../../Proxies/Mrrocpp/Reading.hpp"
#include "ImagePosition.hpp"

namespace Processors {

namespace VisualServoIB {

class IBReading: public Proxies::Mrrocpp::Reading
{
public:
	IBReading()
	{
	}

	IBReading(const IBReading& o)
	{
		objectVisible = o.objectVisible;

	}

	virtual ~IBReading()
	{
	}

	virtual IBReading* clone()
	{
		return new IBReading(*this);
	}

	bool objectVisible;
	Types::ImagePosition imagePosition;


	virtual void printInfo()
	{
		LOG(LNOTICE) << "IBReading::printInfo()\n";
		stringstream ss;
		if (objectVisible) {

		} else {
			ss << "object not visible\n";
		}

		LOG(LNOTICE) << "HomogMatrix:\n" << ss.str() << endl;
	}

	virtual void send(boost::shared_ptr<xdr_oarchive<> > & ar){
		LOG(LNOTICE) << "IBReading::send(): hehehehe\n";
		*ar<<*this;
	}

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object <Reading>(*this);
		LOG(LTRACE) << "IBReading::serialize()\n";
		ar & objectVisible;
		ar & imagePosition;
	}
};

} // namespace VisualServoIB
} // namespace Processors

#endif /* IBREADING_HPP_ */
