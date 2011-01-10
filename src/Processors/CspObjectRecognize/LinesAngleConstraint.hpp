/*
 * LinesAngleConstraint.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef LINESANGLECONSTRAINT_HPP_
#define LINESANGLECONSTRAINT_HPP_

#include "AbstractConstraint.hpp"

namespace Processors {

namespace CspObjectRecognize {

class LinesAngleConstraint: public Processors::CspObjectRecognize::AbstractConstraint
{
public:
	LinesAngleConstraint();
	virtual ~LinesAngleConstraint();
};

}

}

#endif /* LINESANGLECONSTRAINT_HPP_ */
