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

/**
 * Read model description from a file.
 * Model file has the following form.
 * <models>
 * 	<model1 numberOfVertices="3">
 *		<constraints>
 *		<c1 type="" v1="0" v2="1"/>
 *		</constraints>
 * 	</model1>
 * 	<model2>
 * 	</model2>
 * </models>
 */
class ModelsFactory
{
public:
	ModelsFactory();
	virtual ~ModelsFactory();

	void setModelsFilename(const std::string& modelsFilename);
	std::vector<boost::shared_ptr<ObjectModel> > loadModels();
private:
	std::string modelsFilename;

	boost::shared_ptr<ObjectModel> buildObjectModel(const boost::property_tree::ptree& node);

};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* MODELSFACTORY_HPP_ */
