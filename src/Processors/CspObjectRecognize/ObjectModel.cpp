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

ObjectModel::ObjectModel(boost::shared_ptr <CspGraph> graph) :
	graph(graph)
{
	// TODO Auto-generated constructor stub

}

ObjectModel::~ObjectModel()
{
	// TODO Auto-generated destructor stub
}

bool ObjectModel::findInstances(ShapeSegments* segments)
{
	bool found = false;
	if (graph->FindCspShape(segments)) {
		LOG(LFATAL) << "ObjectModel::findInstances(): something found.\n";
		foundObject = graph->GetAllUsedShapesVector();
		found = true;
	} else {
		LOG(LFATAL) << "ObjectModel::findInstances(): nothing found.\n";
	}
	return found;
}

ShapeVector ObjectModel::getFoundObject()
{
	return foundObject;
}

} // namespace CspObjectRecognize
} // namespace Processors
