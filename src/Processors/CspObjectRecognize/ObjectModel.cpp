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

ObjectModel::ObjectModel(boost::shared_ptr<CspGraph> graph) : graph(graph)
{
	// TODO Auto-generated constructor stub

}

ObjectModel::~ObjectModel()
{
	// TODO Auto-generated destructor stub
}

void ObjectModel::findInstances(ShapeSegments* segments)
{
	if(graph->FindCspShape(segments)){
		LOG(LFATAL) << "ObjectModel::findInstances(): something found.\n";
	}
}

} // namespace CspObjectRecognize
} // namespace Processors
