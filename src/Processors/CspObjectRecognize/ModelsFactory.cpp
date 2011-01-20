/*
 * ModelsFactory.cpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "CspGraph.hpp"
#include "ObjectModel.hpp"
#include "ModelsFactory.hpp"
#include "JunctionConstraint.hpp"
#include "LinesAngleConstraint.hpp"

#include "Logger.hpp"

namespace Processors {
namespace CspObjectRecognize {

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::read_xml;

ModelsFactory::ModelsFactory()
{
	// TODO Auto-generated constructor stub

}

ModelsFactory::~ModelsFactory()
{
	// TODO Auto-generated destructor stub
}

void ModelsFactory::setModelsFilename(const std::string& modelsFilename)
{
	this->modelsFilename = modelsFilename;
}

std::vector <boost::shared_ptr <ObjectModel> > ModelsFactory::loadModels()
{
	std::vector <boost::shared_ptr <ObjectModel> > models;
	LOG(LFATAL) << "ModelsFactory::createModels(): " << modelsFilename << " method not yet implemented.\n";

	ptree pt;
	read_xml(modelsFilename, pt, boost::property_tree::xml_parser::no_comments);

	BOOST_FOREACH(ptree::value_type &v, pt.get_child("models"))
				{
					string name = v.first;
					ptree node = v.second;
					LOG(LTRACE) << "Loading model " << name << endl;
					models.push_back(buildObjectModel(node));
					LOG(LTRACE) << "Model " << name << " loaded.\n";
				}

	return models;
}

boost::shared_ptr <ObjectModel> ModelsFactory::buildObjectModel(const ptree& node)
{
	boost::shared_ptr <CspGraph> graph = boost::shared_ptr <CspGraph>(new CspGraph);

	boost::shared_ptr <AbstractConstraint> junctionLine =
			boost::shared_ptr <AbstractConstraint>(new JunctionConstraint);
//	boost::shared_ptr <AbstractConstraint> perpendicularLine =
//			boost::shared_ptr <AbstractConstraint>(new LinesAngleConstraint);
//	boost::shared_ptr <AbstractConstraint> parallelLine =
//			boost::shared_ptr <AbstractConstraint>(new LinesAngleConstraint);
//	boost::shared_ptr <AbstractConstraint> lineLength =
//			boost::shared_ptr <AbstractConstraint>(new LinesAngleConstraint);

	//graph->Clear();
	graph->init(4, 0, true);

	VertexVector vertexVector = graph->GetVertex();

	graph->AddEdge(vertexVector[0], vertexVector[1], junctionLine);
//	graph->AddEdge(vertexVector[0], vertexVector[1], perpendicularLine);

	graph->AddEdge(vertexVector[0], vertexVector[3], junctionLine);
//	graph->AddEdge(vertexVector[0], vertexVector[3], perpendicularLine);

//	graph->AddEdge(vertexVector[0], vertexVector[2], parallelLine);
//	graph->AddEdge(vertexVector[0], vertexVector[2], lineLength);

	graph->AddEdge(vertexVector[1], vertexVector[2], junctionLine);
//	graph->AddEdge(vertexVector[1], vertexVector[2], perpendicularLine);

//	graph->AddEdge(vertexVector[1], vertexVector[3], parallelLine);
//	graph->AddEdge(vertexVector[1], vertexVector[3], lineLength);

	graph->AddEdge(vertexVector[2], vertexVector[3], junctionLine);
//	graph->AddEdge(vertexVector[2], vertexVector[3], perpendicularLine);

	graph->InitInputOutputEdgeMap();
	//przeszukiwanie grafu
	graph->InitSearchGraph();
	//pierwsza sciezka
	graph->AddSearchVertex(vertexVector[0], true);
	graph->AddSearchVertex(vertexVector[1], true);
	graph->AddSearchVertex(vertexVector[2], true);
	graph->AddSearchVertex(vertexVector[3], true);

	boost::shared_ptr <ObjectModel> model = boost::shared_ptr <ObjectModel>(new ObjectModel(graph));
	int numberOfVertices = node.get <int> ("<xmlattr>.numberOfVertices");
	if (numberOfVertices < 2) {
		throw runtime_error("ModelsFactory::buildObjectModel(): numberOfVertices < 2");
	}

	LOG(LTRACE) << "numberOfVertices=" << numberOfVertices << endl;

	return model;
}

} // namespace CspObjectRecognize
} // namespace Processors
