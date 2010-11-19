/*
 * PBReading.hpp
 *
 *  Created on: 19-11-2010
 *      Author: mateusz
 */

#ifndef PBREADING_HPP_
#define PBREADING_HPP_

#include "../../Proxies/Mrrocpp/Reading.hpp"
#include "HomogMatrix.hpp"

namespace Processors {

namespace VisualServoPB {

class PBReading: public Proxies::Mrrocpp::Reading
{
public:
	PBReading()
	{
	}

	PBReading(const PBReading& o)
	{
		objectVisible = o.objectVisible;
		objectPosition = o.objectPosition;
	}

	virtual ~PBReading()
	{
	}

	virtual PBReading* clone()
	{
		return new PBReading(*this);
	}

	bool objectVisible;
	Types::HomogMatrix objectPosition;
};

} // namespace VisualServoPB
} // namespace Processors

#endif /* PBREADING_HPP_ */
