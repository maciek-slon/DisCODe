/*
 * Object3D.hpp
 *
 *  Created on: 17-10-2010
 *      Author: mateusz
 */

#ifndef OBJECT3D_HPP_
#define OBJECT3D_HPP_

#include "Drawable.hpp"

namespace Types {

namespace Objects3D {

struct HomogMatrix
{
	double elements[3][4];
};

class Object3D: public Types::Drawable
{
public:
	virtual ~Object3D()
	{
	}

	void setImagePoints(const cv::vector <cv::Point2f>& imagePoints)
	{
		this->imagePoints = imagePoints;
	}
	const cv::vector <cv::Point2f>& getImagePoints() const
	{
		return imagePoints;
	}

	void setModelPoints(const cv::vector <cv::Point3f>& modelPoints)
	{
		this->modelPoints = modelPoints;
	}

	const cv::vector <cv::Point3f>& getModelPoints() const
	{
		return modelPoints;
	}

	void setPosition(const HomogMatrix& position)
	{
		this->position = position;
	}
	const HomogMatrix& getPosition() const
	{
		return position;
	}

protected:
	cv::vector <cv::Point2f> imagePoints;
	cv::vector <cv::Point3f> modelPoints;
	HomogMatrix position;
};

}

}

#endif /* OBJECT3D_HPP_ */
