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

namespace Types {

class DrawableContainer : public Drawable {
public:
	~DrawableContainer() {}

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

#endif /* DRAWABLECONTAINER_HPP_ */
