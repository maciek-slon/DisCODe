/*
 * ObjectModel.hpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#ifndef OBJECTMODEL_HPP_
#define OBJECTMODEL_HPP_

#include <boost/shared_ptr.hpp>

#include "../GrayImageSegmentation/SegmentedImage.hpp"
#include "CspGraph.hpp"

namespace Processors {
namespace CspObjectRecognize {

class ObjectModel
{
public:
	ObjectModel(boost::shared_ptr<CspGraph> graph);
	virtual ~ObjectModel();
	bool findInstances(ShapeSegments* segments);
	ShapeVector getFoundObject();
private:
	boost::shared_ptr<CspGraph> graph;
	ShapeVector foundObject;
};

typedef std::map <std::string, boost::shared_ptr <ObjectModel> > ModelsMap;

}//: namespace CspObjectRecognize
}//: namespace Processors

#endif /* OBJECTMODEL_HPP_ */
