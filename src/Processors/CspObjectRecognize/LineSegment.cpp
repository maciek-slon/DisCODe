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
	// TODO Auto-generated constructor stub

}

LineSegment::~LineSegment()
{
	// TODO Auto-generated destructor stub
}
Types::Line& LineSegment::getLine()
{
	return line;
}

} // namespace CspObjectRecognize
} // namespace Processors
