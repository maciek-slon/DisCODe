/*
 * LineSegment.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef LINESEGMENT_HPP_
#define LINESEGMENT_HPP_

#include "AbstractShape.hpp"

namespace Processors {

namespace CspObjectRecognize {

class LineSegment: public Processors::CspObjectRecognize::AbstractShape
{
public:
	LineSegment();
	virtual ~LineSegment();
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* LINESEGMENT_HPP_ */
