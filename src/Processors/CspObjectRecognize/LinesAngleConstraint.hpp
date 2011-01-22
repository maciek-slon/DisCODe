/*!
 * \file LinesAngleConstraint.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef LINESANGLECONSTRAINT_HPP_
#define LINESANGLECONSTRAINT_HPP_

#include "AbstractConstraint.hpp"

namespace Processors {

namespace CspObjectRecognize {

/**
 * Constraint for angle between two line segments.
 * TODO: implement method isSatisifed().
 *
 * Properties:
 *
 * <b>minAngle</b>
 *
 * <b>maxAngle</b>
 */
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
