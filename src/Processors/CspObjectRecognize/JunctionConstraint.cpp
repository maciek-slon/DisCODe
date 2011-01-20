/*
 * JunctionConstraint.cpp
 *
 *  Created on: 19-01-2011
 *      Author: mateusz
 */

#include "JunctionConstraint.hpp"

#include "LineSegment.hpp"

namespace Processors {
namespace CspObjectRecognize {

JunctionConstraint::JunctionConstraint()
{
}

JunctionConstraint::~JunctionConstraint()
{
}

bool JunctionConstraint::isSatisifed(boost::shared_ptr<AbstractShape> first, boost::shared_ptr<AbstractShape> second)
{
	LineSegment *firstLine = dynamic_cast <LineSegment*> (first.get());
	LineSegment *secondLine = dynamic_cast <LineSegment*> (second.get());
	if (firstLine == NULL || secondLine == NULL) {
		return false;
	}

	double maxDistance = 5.0;

	cv::Point line1p1 = firstLine->getLine().getP1();
	cv::Point line1p2 = firstLine->getLine().getP2();
	cv::Point line2p1 = secondLine->getLine().getP1();
	cv::Point line2p2 = secondLine->getLine().getP2();

	if(
			dist(line1p1, line2p1) <= maxDistance ||
			dist(line1p1, line2p2) <= maxDistance ||
			dist(line1p2, line2p1) <= maxDistance ||
			dist(line1p2, line2p2) <= maxDistance
	){
		return true;
	}

	return false;
}

} // namespace CspObjectRecognize
} // namespace Processors
