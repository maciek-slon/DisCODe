/*
 * LinesLengthConstraint.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef LINESLENGTHCONSTRAINT_HPP_
#define LINESLENGTHCONSTRAINT_HPP_

#include "AbstractConstraint.hpp"

namespace Processors {

namespace CspObjectRecognize {

class LinesLengthConstraint: public Processors::CspObjectRecognize::AbstractConstraint
{
public:
	LinesLengthConstraint();
	virtual ~LinesLengthConstraint();
};

}

}

#endif /* LINESLENGTHCONSTRAINT_HPP_ */
