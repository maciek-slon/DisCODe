/*!
 * \file AbstractShape.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef ABSTRACTSHAPE_HPP_
#define ABSTRACTSHAPE_HPP_

#include <vector>
#include <boost/shared_ptr.hpp>
#include <cv.h>

namespace Processors {
namespace CspObjectRecognize {

/**
 * Abstract shape represents any shape which is treated as a vertex in CspGraph.
 * There may be constraints between vertices (shapes) which has to be met.
 * Constraints are treated as edges in CspGraph.
 *
 * TODO: When adding more specializations of this class replace getType() with another method for determining type of shape.
 */
class AbstractShape
{
public:
	/** Constructor. */
	AbstractShape();
	/** Destructor. */
	virtual ~AbstractShape();
	/**
	 * TODO: remove, it is required by CspGraph implementation, but has to be replaced with something more object-oriented-like.
	 * @return type of shape set permanently to 0
	 */
	int getType();

	/**
	 * Draw shape in OpenCV image.
	 * @param image
	 * @param color
	 */
	virtual void draw(cv::Mat &image, CvScalar color) = 0;
};

/** Collection of shapes used by CspGraph. */
typedef std::vector <boost::shared_ptr <AbstractShape> > ShapeVector;

}
}

#endif /* ABSTRACTSHAPE_HPP_ */
