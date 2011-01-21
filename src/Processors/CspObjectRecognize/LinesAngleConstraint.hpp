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
	virtual bool isSatisifed(boost::shared_ptr<AbstractShape> first, boost::shared_ptr<AbstractShape> second);
};

}

}

#endif /* LINESANGLECONSTRAINT_HPP_ */
