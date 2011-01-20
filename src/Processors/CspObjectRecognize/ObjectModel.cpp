/*
 * ObjectModel.cpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#include "ObjectModel.hpp"

#include "Logger.hpp"

namespace Processors {

namespace CspObjectRecognize {

ObjectModel::ObjectModel(boost::shared_ptr <CspGraph> graph, const boost::property_tree::ptree& node) :
	graph(graph)
{
	drawColor = CV_RGB(node.get <int> ("drawColor.R"), node.get <int> ("drawColor.G"), node.get <int> ("drawColor.B"));
}

ObjectModel::~ObjectModel()
{
	// TODO Auto-generated destructor stub
}

bool ObjectModel::findInstances(ShapeSegments* segments)
{
	bool found = false;
	if (graph->FindCspShape(segments)) {
		LOG(LTRACE) << "ObjectModel::findInstances(): something found.";
		foundObject = graph->GetAllUsedShapesVector();
		found = true;
	} else {
		LOG(LTRACE) << "ObjectModel::findInstances(): nothing found.\n";
	}
	return found;
}

ShapeVector ObjectModel::getFoundObject()
{
	return foundObject;
}

CvScalar ObjectModel::getDrawColor()
{
	return drawColor;
}

} // namespace CspObjectRecognize
} // namespace Processors
