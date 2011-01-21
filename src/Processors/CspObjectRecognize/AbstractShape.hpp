/*
 * AbstractShape.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef ABSTRACTSHAPE_HPP_
#define ABSTRACTSHAPE_HPP_

#include <vector>
#include <boost/shared_ptr.hpp>
#include <cv.h>

namespace Processors {

namespace CspObjectRecognize {

class AbstractShape
{
public:
	AbstractShape();
	virtual ~AbstractShape();
	int getType();
	virtual void draw(cv::Mat &image, CvScalar color) = 0;
};

typedef std::vector <boost::shared_ptr <AbstractShape> > ShapeVector;

}
}

#endif /* ABSTRACTSHAPE_HPP_ */
