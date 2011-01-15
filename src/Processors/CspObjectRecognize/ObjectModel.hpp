/*
 * ObjectModel.hpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#ifndef OBJECTMODEL_HPP_
#define OBJECTMODEL_HPP_

#include "../GrayImageSegmentation/SegmentedImage.hpp"

namespace Processors {
namespace CspObjectRecognize {

class ObjectModel
{
public:
	ObjectModel();
	virtual ~ObjectModel();
	void findInstances(const Types::Segmentation::SegmentedImage& si);
};

}//: namespace CspObjectRecognize
}//: namespace Processors

#endif /* OBJECTMODEL_HPP_ */
