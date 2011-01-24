/*
 * ObjectInstance.cpp
 *
 *  Created on: 20-01-2011
 *      Author: mateusz
 */

#include <boost/foreach.hpp>

#include "ObjectInstance.hpp"

namespace Processors {
namespace CspObjectRecognize {

ObjectInstance::ObjectInstance(boost::shared_ptr <ObjectModel> model, const ShapeVector& shape) :
	model(model), shape(shape)
{
}

ObjectInstance::~ObjectInstance()
{
}

void ObjectInstance::draw(cv::Mat& image, CvScalar color, int offsetX, int offsetY)
{
	BOOST_FOREACH(boost::shared_ptr <AbstractShape> s,shape)
				{
					s->draw(image, model->getDrawColor());
				}
}

ObjectInstance * ObjectInstance::clone()
{
	return new ObjectInstance(*this);
}

}
}
