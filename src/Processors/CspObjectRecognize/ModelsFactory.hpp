/*!
 * \file ModelsFactory.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef MODELSFACTORY_HPP_
#define MODELSFACTORY_HPP_

#include <map>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

#include "ObjectModel.hpp"

namespace Processors {
namespace CspObjectRecognize {

/**
 * Read model description from a file.
 * Model file has the following form.
\code
<?xml version="1.0" encoding="UTF-8"?>
<models>
	<Triangle numberOfVertices="3">
		<drawColor>
			<R>0</R>
			<G>255</G>
			<B>255</B>
		</drawColor>
		<constraints>
			<c0 type="junction" from="0" to="1"></c0>
			<c1 type="junction" from="1" to="2"></c1>
			<c2 type="junction" from="2" to="0"></c2>
		</constraints>
	</Triangle>

	<Quadrilateral numberOfVertices="4">
		<drawColor>
			<R>255</R>
			<G>0</G>
			<B>255</B>
		</drawColor>
		<constraints>
			<c0 type="junction" from="0" to="1"></c0>
			<c1 type="junction" from="1" to="2"></c1>
			<c2 type="junction" from="2" to="3"></c2>
			<c3 type="junction" from="3" to="0"></c3>
		</constraints>
	</Quadrilateral>
	<Pentagon numberOfVertices="5">
		<drawColor>
			<R>255</R>
			<G>255</G>
			<B>0</B>
		</drawColor>
		<constraints>
			<c0 type="junction" from="0" to="1"></c0>
			<c1 type="junction" from="1" to="2"></c1>
			<c2 type="junction" from="2" to="3"></c2>
			<c3 type="junction" from="3" to="4"></c3>
			<c4 type="junction" from="4" to="0"></c4>
		</constraints>
	</Pentagon>

</models>
\endcode
 */
class ModelsFactory
{
public:
	/**
	 * Create Models factory.
	 */
	ModelsFactory();
	/**
	 * Destructor
	 */
	virtual ~ModelsFactory();

	/**
	 * Sets models database filename.
	 * Call it before calling loadModels.
	 * @param modelsFilename
	 */
	void setModelsFilename(const std::string& modelsFilename);

	/**
	 * Read objects database.
	 * @throw std::exception throws exception if something's wrong with database file.
	 * @return whole objects' database.
	 */
	ModelsMap loadModels();
private:
	std::string modelsFilename;

	boost::shared_ptr<ObjectModel> buildObjectModel(const boost::property_tree::ptree& node);
	boost::shared_ptr<AbstractConstraint> buildConstraint(const boost::property_tree::ptree& node);
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* MODELSFACTORY_HPP_ */
