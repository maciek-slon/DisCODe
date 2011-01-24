/*!
 * \file ObjectInstance.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
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

/**
 * Instance of object found by CspGraph.
 */
class ObjectInstance: public Types::Drawable
{
public:
	/**
	 * Create object instance.
	 * @param model
	 * @param shape
	 */
	ObjectInstance(boost::shared_ptr <ObjectModel> model, const ShapeVector& shape);
	virtual ~ObjectInstance();

	/**
	 * Draw object on the image.
	 * @param image
	 * @param color
	 * @param offsetX
	 * @param offsetY
	 */
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
