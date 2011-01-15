/*
 * ShapeRecognize.cpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#include "ShapeRecognize.hpp"

#include <boost/foreach.hpp>

#include "Logger.hpp"

namespace Processors {
namespace CspObjectRecognize {

ShapeRecognize::ShapeRecognize()
{
}

ShapeRecognize::~ShapeRecognize()
{
}

void ShapeRecognize::setModels(std::vector <boost::shared_ptr <ObjectModel> > models)
{
	this->models = models;
}

void ShapeRecognize::recognize(const Types::Segmentation::SegmentedImage& si)
{
	LOG(LFATAL) << "ShapeRecognize::recognize()\n";
	BOOST_FOREACH(boost::shared_ptr<ObjectModel> m, models){
		m->findInstances(si);
	}
}

} // namespace CspObjectRecognize
} // namespace Processors
