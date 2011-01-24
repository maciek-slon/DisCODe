/*!
 * \file ObjectModel.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef OBJECTMODEL_HPP_
#define OBJECTMODEL_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cv.h>

#include "../GrayImageSegmentation/SegmentedImage.hpp"
#include "CspGraph.hpp"

namespace Processors {
namespace CspObjectRecognize {

/**
 * Model of object.
 * Contains CspGraph.
 * Properties <b>drawColor.R</b>, <b>drawColor.G</b>, <b>drawColor.B</b> specify which color to draw.
 */
class ObjectModel
{
public:
	/**
	 * Create object model.
	 * @param graph
	 * @param node read configuration
	 */
	ObjectModel(boost::shared_ptr<CspGraph> graph, const boost::property_tree::ptree& node);
	virtual ~ObjectModel();

	/**
	 * Find instances using CspGraph search.
	 * @param segments
	 * @return
	 */
	bool findInstances(ShapeSegments* segments);

	/**
	 * Get objects found by CspGraph.
	 * @return
	 */
	ShapeVector getFoundObject();

	/**
	 * Get draw color.
	 * @return
	 */
	CvScalar getDrawColor();
private:
	boost::shared_ptr<CspGraph> graph;
	ShapeVector foundObject;
	CvScalar drawColor;
};

typedef std::map <std::string, boost::shared_ptr <ObjectModel> > ModelsMap;

}//: namespace CspObjectRecognize
}//: namespace Processors

#endif /* OBJECTMODEL_HPP_ */
