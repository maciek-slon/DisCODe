/*!
 * \file ShapeRecognize.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef SHAPERECOGNIZE_HPP_
#define SHAPERECOGNIZE_HPP_

#include <vector>
#include <boost/shared_ptr.hpp>
#include <map>

#include "ObjectModel.hpp"
#include "ObjectInstance.hpp"

#include "../GrayImageSegmentation/SegmentedImage.hpp"

namespace Processors {
namespace CspObjectRecognize {

/**
 * Recognizes shapes from given objects models.
 */
class ShapeRecognize
{
public:
	ShapeRecognize();
	virtual ~ShapeRecognize();

	/**
	 * Set models to recognize.
	 * Every model will be checked.
	 * @param models
	 */
	void setModels(const std::map <std::string, boost::shared_ptr <ObjectModel> >& models);

	/**
	 * Recognize objects in segmented image.
	 * @param si segmented image.
	 * @return recognize object instances.
	 */
	ObjectInstanceVector recognize(Types::Segmentation::SegmentedImage& si);
private:
	std::map <std::string, boost::shared_ptr <ObjectModel> > models;
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* SHAPERECOGNIZE_HPP_ */
