/*!
 * \file Rectangle.hpp
 * \brief File containing Rectangle type
 */

#ifndef RECTANGLE_HPP_
#define RECTANGLE_HPP_

#include "Drawable.hpp"

#include <cv.h>

namespace Types {

class Rectangle : public Drawable {
public:
	Rectangle(int x, int y, int w, int h) : p1(x,y), p2(x+w, y+h)
	{}

    Rectangle(const Rectangle & rhs) : p1(rhs.p1), p2(rhs.p2) {};

	virtual ~Rectangle() {}

	virtual void draw(cv::Mat & image, CvScalar color, int offsetX = 0, int offsetY = 0) {
		cv::rectangle(image, p1, p2, color, 2);
	}

	virtual Drawable * clone() {
		return new Rectangle(*this);
	}

private:
	const cv::Point p1, p2;
};

} //: namespace Types

#endif /* RECTANGLE_HPP_ */
