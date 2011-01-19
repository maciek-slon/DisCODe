/*
 * ModelsFactory.cpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "ModelsFactory.hpp"

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
	boost::shared_ptr <ObjectModel> model = boost::shared_ptr <ObjectModel>(new ObjectModel);
	int numberOfVertices = node.get<int>("<xmlattr>.numberOfVertices");
	if(numberOfVertices < 2){
		throw runtime_error("ModelsFactory::buildObjectModel(): numberOfVertices < 2");
	}

	LOG(LTRACE) << "numberOfVertices="<<numberOfVertices<<endl;

	return model;
}

} // namespace CspObjectRecognize
} // namespace Processors
