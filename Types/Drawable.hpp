#ifndef DRAWABLE_HPP_
#define DRAWABLE_HPP_

#include <cxtypes.h>

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
	virtual void draw(IplImage *image, CvScalar color, int offsetX = 0, int offsetY = 0) = 0;
};

} //: namespace Types

#endif /* DRAWABLE_HPP_ */
