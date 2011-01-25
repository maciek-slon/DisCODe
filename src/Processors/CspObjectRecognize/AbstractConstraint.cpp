/*
 * AbstractConstraint.cpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#include "AbstractConstraint.hpp"

namespace Processors {

namespace CspObjectRecognize {

AbstractConstraint::AbstractConstraint()
{
}

AbstractConstraint::~AbstractConstraint()
{
}

double AbstractConstraint::dist(cv::Point p1, cv::Point p2){
	return sqrt(
		(p2.x - p1.x) * (p2.x - p1.x) +
		(p2.y - p1.y) * (p2.y - p1.y)
		);
}

} // namespace Processors
} // namespace CspObjectRecognize
