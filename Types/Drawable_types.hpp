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
#include <vector>

namespace Types {

class Ellipse : public Drawable {
public:
	Ellipse(const cv::Point2f& _center, const cv::Size2f& _size, float _angle = 0.0f) : rect_(_center, _size, _angle)
	{}

    Ellipse(const CvBox2D& box) : rect_(box) {};

    Ellipse(const cv::RotatedRect & rhs) : rect_(rhs) {};

    Ellipse(const Ellipse & rhs) : rect_(rhs.rect_) {};

	virtual void draw(cv::Mat & image, CvScalar color, int offsetX = 0, int offsetY = 0) {
		//cv::ellipse(image, rect_, color);
		cv::ellipse(image, cv::Point(rect_.center.x, rect_.center.y), cv::Size(rect_.size.width, rect_.size.height), rect_.angle, 0, 360, color, 2);
	}

	virtual Drawable * clone() {
		return new Ellipse(*this);
	}

private:
	cv::RotatedRect rect_;
};

class DrawableContainer : public Drawable {
public:
	virtual void draw(cv::Mat & image, CvScalar color, int offsetX = 0, int offsetY = 0) {
		for (int i = 0; i < items.size(); ++i)
			items[i]->draw(image, color, offsetX, offsetY);
	}

	void add(Drawable * it) {
		items.push_back(it);
	}

	virtual Drawable * clone() {
		DrawableContainer * ret = new DrawableContainer;
		for (int i = 0; i < items.size(); ++i)
			ret->add(items[i]->clone());

		return ret;
	}

private:
	std::vector<Drawable*> items;
};

} //: namespace Types

#endif /* DRAWABLE_TYPES_HPP_ */
