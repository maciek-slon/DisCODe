#ifndef DRAWABLE_HPP_
#define DRAWABLE_HPP_

#include <cv.h>

namespace Types {

/*!
 * \class Drawable
 *
 * \brief Interface used for representation of drawable objects.
 *
 * Each object that implements this interface must implement draw method.
 *
 * \author mstefanc
 */
class Drawable {
public:
	virtual ~Drawable() {}

	virtual void draw(cv::Mat& image, CvScalar color, int offsetX = 0, int offsetY = 0) = 0;

	virtual Drawable * clone() {
		return NULL;
	}
};

} //: namespace Types

#endif /* DRAWABLE_HPP_ */
