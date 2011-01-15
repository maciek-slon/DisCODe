/*
 * ModelsFactory.cpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#include "ModelsFactory.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CspObjectRecognize {

using namespace std;

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
	return models;
}

} // namespace CspObjectRecognize
} // namespace Processors
