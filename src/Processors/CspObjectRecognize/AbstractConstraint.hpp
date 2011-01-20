/*
 * AbstractConstraint.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef ABSTRACTCONSTRAINT_HPP_
#define ABSTRACTCONSTRAINT_HPP_

#include <boost/shared_ptr.hpp>
#include <cv.h>

#include "AbstractShape.hpp"


namespace Processors {
namespace CspObjectRecognize {

class AbstractShape;

class AbstractConstraint
{
public:
	AbstractConstraint();
	virtual ~AbstractConstraint();
	virtual bool isSatisifed(boost::shared_ptr<AbstractShape> first, boost::shared_ptr<AbstractShape> second) = 0;
protected:
	double dist(cv::Point p1, cv::Point p2);
};

} // namespace Processors
} // namespace CspObjectRecognize

#endif /* ABSTRACTCONSTRAINT_HPP_ */
