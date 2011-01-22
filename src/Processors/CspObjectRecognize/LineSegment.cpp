/*
 * LineSegment.cpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#include "LineSegment.hpp"

namespace Processors {

namespace CspObjectRecognize {

LineSegment::LineSegment(const Types::Line& line) :
	line(line)
{
}

LineSegment::~LineSegment()
{
}
Types::Line& LineSegment::getLine()
{
	return line;
}

void LineSegment::draw(cv::Mat &image, CvScalar color)
{
	line.draw(image, color);
}

} // namespace CspObjectRecognize
} // namespace Processors
