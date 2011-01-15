/*
 * ModelsFactory.hpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#ifndef MODELSFACTORY_HPP_
#define MODELSFACTORY_HPP_

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

#include "ObjectModel.hpp"

namespace Processors {
namespace CspObjectRecognize {

class ModelsFactory
{
public:
	ModelsFactory();
	virtual ~ModelsFactory();

	void setModelsFilename(const std::string& modelsFilename);
	std::vector<boost::shared_ptr<ObjectModel> > loadModels();
private:
	std::string modelsFilename;

};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* MODELSFACTORY_HPP_ */
