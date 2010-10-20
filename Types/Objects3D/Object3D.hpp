/*
 * Object3D.hpp
 *
 *  Created on: 17-10-2010
 *      Author: mateusz
 */

#ifndef OBJECT3D_HPP_
#define OBJECT3D_HPP_

#include <stdexcept>

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
	Object3D() :
		imagePointsSet(false), modelPointsSet(false), positionSet(false)
	{
	}

	Object3D(const Object3D& o)
	{
		imagePointsSet = o.imagePointsSet;
		modelPointsSet = o.imagePointsSet;
		positionSet = o.positionSet;
		imagePoints = o.imagePoints;
		modelPoints = o.modelPoints;
		position = o.position;
	}

	virtual ~Object3D()
	{
	}

	void setImagePoints(const cv::vector <cv::Point2f>& imagePoints)
	{
		this->imagePoints = imagePoints;
		imagePointsSet = true;
		positionSet = false;
	}
	const cv::vector <cv::Point2f>& getImagePoints() const
	{
		if (!imagePointsSet) {
			throw std::logic_error("imagePoints has not been set.");
		}
		return imagePoints;
	}

	void setModelPoints(const cv::vector <cv::Point3f>& modelPoints)
	{
		this->modelPoints = modelPoints;
		modelPointsSet = true;
		positionSet = false;
	}

	const cv::vector <cv::Point3f>& getModelPoints() const
	{
		if (!modelPointsSet) {
			throw std::logic_error("modelPoints has not been set.");
		}
		return modelPoints;
	}

	void setPosition(const HomogMatrix& position)
	{
		this->position = position;
		positionSet = true;
	}

	const HomogMatrix& getPosition() const
	{
		if (!positionSet) {
			throw std::logic_error("position has not been set.");
		}
		return position;
	}

protected:
	bool imagePointsSet;
	bool modelPointsSet;
	bool positionSet;

	cv::vector <cv::Point2f> imagePoints;
	cv::vector <cv::Point3f> modelPoints;
	HomogMatrix position;
};

}

}

#endif /* OBJECT3D_HPP_ */
