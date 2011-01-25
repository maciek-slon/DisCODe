/*!
 * \file LineSegment.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef LINESEGMENT_HPP_
#define LINESEGMENT_HPP_

#include <cv.h>

#include "AbstractShape.hpp"
#include "Types/Line.hpp"

namespace Processors {
namespace CspObjectRecognize {

/**
 * The most primitive shape for CspGraph to recognize (I guess).
 * Consists of line segment.
 */
class LineSegment: public Processors::CspObjectRecognize::AbstractShape
{
public:
	/**
	 * Create line segment.
	 * @param line
	 */
	LineSegment(const Types::Line& line);

	virtual ~LineSegment();

	/**
	 * Get line segment.
	 * @return
	 */
	Types::Line& getLine();

	/**
	 * Draw on image.
	 * @param image
	 * @param color
	 */
	virtual void draw(cv::Mat &image, CvScalar color);
private:
	/** Line. */
	Types::Line line;
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* LINESEGMENT_HPP_ */
