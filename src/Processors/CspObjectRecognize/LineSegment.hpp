/*
 * LineSegment.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef LINESEGMENT_HPP_
#define LINESEGMENT_HPP_

#include <cv.h>

#include "AbstractShape.hpp"
#include "Types/Line.hpp"

namespace Processors {

namespace CspObjectRecognize {

class LineSegment: public Processors::CspObjectRecognize::AbstractShape
{
public:
	LineSegment(const Types::Line& line);
	virtual ~LineSegment();
	Types::Line& getLine();
	virtual void draw(cv::Mat &image, CvScalar color);
private:
	Types::Line line;
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* LINESEGMENT_HPP_ */
