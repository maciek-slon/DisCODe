/*!
 * \file DrawableContainer.hpp
 * \brief File containing DrawableContainer type
 *
 * \author mstefanc
 * \date 20.08.2010
 */

#ifndef DRAWABLECONTAINER_HPP_
#define DRAWABLECONTAINER_HPP_

#include "Drawable.hpp"

#include <cv.h>
#include <vector>

#include <boost/foreach.hpp>

namespace Types {

class DrawableContainer : public Drawable {
public:
	~DrawableContainer() {}

	virtual void draw(cv::Mat & image, CvScalar color, int offsetX = 0, int offsetY = 0) {
		BOOST_FOREACH(Drawable * item, items) {
			item->draw(image, color, offsetX, offsetY);
		}
	}

	void add(Drawable * it) {
		items.push_back(it);
	}

	virtual Drawable * clone() {
		DrawableContainer * ret = new DrawableContainer;
		BOOST_FOREACH(Drawable * item, items) {
			ret->add(item->clone());
		}

		return ret;
	}

private:
	std::vector<Drawable*> items;
};

} //: namespace Types

#endif /* DRAWABLECONTAINER_HPP_ */
