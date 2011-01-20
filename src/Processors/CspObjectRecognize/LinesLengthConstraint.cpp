/*
 * LinesLengthConstraint.cpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#include "LinesLengthConstraint.hpp"

namespace Processors {

namespace CspObjectRecognize {

LinesLengthConstraint::LinesLengthConstraint()
{
	// TODO Auto-generated constructor stub

}

LinesLengthConstraint::~LinesLengthConstraint()
{
	// TODO Auto-generated destructor stub
}

bool LinesLengthConstraint::isSatisifed(boost::shared_ptr <AbstractShape> first, boost::shared_ptr <AbstractShape> second)
{
	return false;
}

}

}
