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
    read_xml(modelsFilename, pt);

    BOOST_FOREACH(ptree::value_type &v, pt.get_child("models")){
    	LOG(LFATAL) << "ModelsFactory::loadModels(): " << v.second.data();
    }

	return models;
}

} // namespace CspObjectRecognize
} // namespace Processors
