/*
 * ShapeRecognize.hpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#ifndef SHAPERECOGNIZE_HPP_
#define SHAPERECOGNIZE_HPP_

#include <vector>
#include <boost/shared_ptr.hpp>
#include "ObjectModel.hpp"

#include "../GrayImageSegmentation/SegmentedImage.hpp"

namespace Processors {
namespace CspObjectRecognize {

class ShapeRecognize
{
public:
	ShapeRecognize();
	virtual ~ShapeRecognize();

	/**
	 * Set models to recognize.
	 * Every model will be checked.s
	 * @param models
	 */
	void setModels(std::vector<boost::shared_ptr<ObjectModel> > models);

	void recognize(Types::Segmentation::SegmentedImage& si);
private:
	std::vector<boost::shared_ptr<ObjectModel> > models;
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* SHAPERECOGNIZE_HPP_ */
