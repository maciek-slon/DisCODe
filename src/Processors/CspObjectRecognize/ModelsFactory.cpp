/*
 * ModelsFactory.cpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>

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
}

ModelsFactory::~ModelsFactory()
{
}

void ModelsFactory::setModelsFilename(const std::string& modelsFilename)
{
	this->modelsFilename = modelsFilename;
}

ModelsMap ModelsFactory::loadModels()
{
	ModelsMap models;
	LOG(LFATAL) << "ModelsFactory::createModels(): " << modelsFilename << " method not yet implemented.\n";

	ptree pt;
	read_xml(modelsFilename, pt, boost::property_tree::xml_parser::no_comments);

	BOOST_FOREACH(ptree::value_type &v, pt.get_child("models"))
				{
					string name = v.first;
					ptree node = v.second;
					LOG(LTRACE) << "Loading model " << name << endl;
					if (models.find(name) != models.end()) {
						throw runtime_error("Model " + name + " is duplicated.");
					}
					models[name] = buildObjectModel(node);
					LOG(LTRACE) << "Model " << name << " loaded.\n";
				}

	return models;
}

boost::shared_ptr <ObjectModel> ModelsFactory::buildObjectModel(const ptree& node)
{
	int numberOfVertices = node.get <int> ("<xmlattr>.numberOfVertices");
	if (numberOfVertices < 2) {
		throw runtime_error("ModelsFactory::buildObjectModel(): numberOfVertices < 2");
	}

	LOG(LFATAL) << "numberOfVertices=" << numberOfVertices << endl;
	boost::shared_ptr <CspGraph> graph = boost::shared_ptr <CspGraph>(new CspGraph);
	graph->init(numberOfVertices, 0, true);
	VertexVector vertexVector = graph->GetVertex();

	// retreive constraints
	BOOST_FOREACH(const ptree::value_type &v, node.get_child("constraints"))
				{
					string constraintName = v.first;
					ptree constraintNode = v.second;
					LOG(LFATAL) << "Loading constraint: " << constraintName;
					int fromIdx = constraintNode.get <int> ("<xmlattr>.from");
					int toIdx = constraintNode.get <int> ("<xmlattr>.to");
					if (fromIdx < 0 || fromIdx >= numberOfVertices) {
						throw runtime_error("ModelsFactory::buildObjectModel(): from out of range.");
					}
					if (toIdx < 0 || toIdx >= numberOfVertices) {
						throw runtime_error("ModelsFactory::buildObjectModel(): to out of range.");
					}
					graph->AddEdge(vertexVector[fromIdx], vertexVector[toIdx], buildConstraint(constraintNode));

					LOG(LTRACE) << "constraint " << constraintName << " loaded.\n";
				}

	graph->InitInputOutputEdgeMap();
	//przeszukiwanie grafu
	graph->InitSearchGraph();
	//pierwsza sciezka
	for (int i = 0; i < numberOfVertices; ++i) {
		graph->AddSearchVertex(vertexVector[i], true);
	}

	boost::shared_ptr <ObjectModel> model = boost::shared_ptr <ObjectModel>(new ObjectModel(graph, node));

	return model;
}

boost::shared_ptr <AbstractConstraint> ModelsFactory::buildConstraint(const boost::property_tree::ptree& node)
{
	boost::shared_ptr <AbstractConstraint> constraint;

	string type = node.get <string> ("<xmlattr>.type");

	if (type == "junction") {
		constraint = boost::shared_ptr <AbstractConstraint>(new JunctionConstraint(node));
	} else {
		throw runtime_error("ModelsFactory::buildConstraint(): unknown type: \"" + type + "\".");
	}

	return constraint;
}

} // namespace CspObjectRecognize
} // namespace Processors
