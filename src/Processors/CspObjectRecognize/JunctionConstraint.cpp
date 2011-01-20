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
//	LineSegment *firstLine = dynamic_cast <LineSegment> (first);
//	LineSegment *secondLine = dynamic_cast <LineSegment> (second);
//	if (firstLine == NULL || secondLine == NULL) {
//		return false;
//	}
//
//	double minLength = 5.0;
//
//	if (firstLine->GetStartPoint() == secondLine->GetStartPoint() || firstLine->GetEndPoint()
//			== secondLine->GetEndPoint() || firstLine->GetStartPoint() == secondLine->GetEndPoint()
//			|| firstLine->GetEndPoint() == secondLine->GetStartPoint()) {
//		return true;
//	} else if (Math::PointLength(firstLine->GetStartPoint(), secondLine->GetStartPoint()) < minLength
//			|| Math::PointLength(firstLine->GetEndPoint(), secondLine->GetEndPoint()) < minLength
//			|| Math::PointLength(firstLine->GetStartPoint(), secondLine->GetEndPoint()) < minLength
//			|| Math::PointLength(firstLine->GetEndPoint(), secondLine->GetStartPoint()) < minLength) {
//		return true;
//	}

	return false;
}

} // namespace CspObjectRecognize
} // namespace Processors
