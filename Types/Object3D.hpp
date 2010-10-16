/*
 * Object3D.hpp
 *
 *  Created on: 16-10-2010
 *      Author: mateusz
 */

#ifndef OBJECT3D_HPP_
#define OBJECT3D_HPP_

#include <cv.h>

namespace Types {

typedef double HomogMatrix[3][4];

class Object3D
{
public:
	virtual ~Object3D();
	virtual const cv::vector<cv::Point2f>& getImagePoints() const = 0;
	virtual const cv::vector<cv::Point3f>& getModelPoints() const = 0;
	virtual void setPosition(HomogMatrix position) = 0;
	virtual const HomogMatrix getPosition() const = 0;
};

}

#endif /* OBJECT3D_HPP_ */
