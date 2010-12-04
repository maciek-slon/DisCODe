/*
 * Segment.cpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#include "Segment.hpp"

namespace Types {
namespace Segmentation {

Segment::Segment(cv::Point startingPoint, MaskType segmentClass) :
	startingPoint(startingPoint), segmentClass(segmentClass), areaComputed(false)
{
}

Segment::Segment(cv::Point startingPoint, MaskType segmentClass, int area) :
	startingPoint(startingPoint), segmentClass(segmentClass), areaComputed(false), area(area)
{

}

Segment::Segment(const Segment& o)
{
	startingPoint = o.startingPoint;
	segmentClass = o.segmentClass;
	areaComputed = o.areaComputed;
	area = o.area;
}

Segment::~Segment()
{
}

cv::Point Segment::getStartingPoint()
{
	return startingPoint;
}
MaskType Segment::getSegmentClass()
{
	return segmentClass;
}

} // namespace Segmentation
} // namespace Types
