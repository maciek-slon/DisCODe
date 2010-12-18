/*
 * Contour.hpp
 *
 *  Created on: 09-12-2010
 *      Author: mateusz
 */

#ifndef CONTOUR_HPP_
#define CONTOUR_HPP_

#include <vector>

#include "Drawable.hpp"

namespace Types {

class Contour: public Drawable
{
public:
	virtual ~Contour()
	{
	}

	virtual void draw(cv::Mat& image, CvScalar color, int offsetX = 0, int offsetY = 0)
	{
		cv::drawContours(image, contours, -1, color);
	}

	virtual Contour * clone()
	{
		return new Contour(*this);
	}

	void setContours(const std::vector <std::vector <cv::Point> >& contours)
	{
		this->contours = contours;
	}
private:
	std::vector <std::vector <cv::Point> > contours;
};

}

#endif /* CONTOUR_HPP_ */
