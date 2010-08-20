/*!
 * \file Drawable_types.hpp
 * \brief File containing all types that can be drawn (implementing Types::Drawable)
 *
 * \author mstefanc
 * \date 20.08.2010
 */

#ifndef DRAWABLE_TYPES_HPP_
#define DRAWABLE_TYPES_HPP_

#include "Drawable.hpp"

#include <cv.h>

namespace Types {

class Ellipse : public Drawable {
public:
	Ellipse(const Point2f& _center, const Size2f& _size, float _angle = 0.0f) : rect(_center, _size, _angle)
	{}

    Ellipse(const CvBox2D& box) : rect_(box) {};

    Ellipse(const cv::RotatedRect & rhs) : rect_(rhs) {};

    Ellipse(const Ellipse & rhs) : recv_(rhs.rect_) {};

	virtual void draw(cv::Mat & image, CvScalar color, int offsetX = 0, int offsetY = 0) {
		cv::ellipse(image, rect_, color);
	}

private:
	cv::RotatedRect rect_;
};

} //: namespace Types

#endif /* DRAWABLE_TYPES_HPP_ */
