/*
 * ObjectInstance.hpp
 *
 *  Created on: 20-01-2011
 *      Author: mateusz
 */

#ifndef OBJECTINSTANCE_HPP_
#define OBJECTINSTANCE_HPP_

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Types/Drawable.hpp"
#include "ObjectModel.hpp"
#include "AbstractShape.hpp"

namespace Processors {
namespace CspObjectRecognize {

class ObjectInstance: public Types::Drawable
{
public:
	ObjectInstance(boost::shared_ptr <ObjectModel> model, const ShapeVector& shape);
	virtual ~ObjectInstance();

	virtual void draw(cv::Mat& image, CvScalar color, int offsetX = 0, int offsetY = 0);

	virtual ObjectInstance * clone();

private:
	boost::shared_ptr <ObjectModel> model;
	ShapeVector shape;
};

typedef std::vector<boost::shared_ptr<ObjectInstance> > ObjectInstanceVector;

}
}

#endif /* OBJECTINSTANCE_HPP_ */
