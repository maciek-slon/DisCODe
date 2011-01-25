/*
 * ShapeRecognize.cpp
 *
 *  Created on: 15-01-2011
 *      Author: mateusz
 */

#include "ShapeRecognize.hpp"

#include <boost/foreach.hpp>

#include "../GrayImageSegmentation/Segment.hpp"
#include "LineSegment.hpp"

#include "Logger.hpp"
#include <string>

namespace Processors {
namespace CspObjectRecognize {

using Types::Segmentation::Segment;

ShapeRecognize::ShapeRecognize()
{
}

ShapeRecognize::~ShapeRecognize()
{
}

void ShapeRecognize::setModels(const std::map <std::string, boost::shared_ptr <ObjectModel> >& models)
{
	this->models = models;
}

ObjectInstanceVector ShapeRecognize::recognize(Types::Segmentation::SegmentedImage& si)
{
	ObjectInstanceVector instances;
	BOOST_FOREACH(Segment& s, si.segments)
				{
					ShapeSegments segments;
					BOOST_FOREACH(Types::Line& line, *s.getLineSegments())
								{
									boost::shared_ptr <AbstractShape> shape =
											boost::shared_ptr <AbstractShape>(new LineSegment(line));
									segments.push_back(shape);
								}
					LOG(LDEBUG) << "ShapeRecognize::recognize(): trying to recognize segoment: segments.size() = "
							<< segments.size();



					BOOST_FOREACH(ModelsMap::value_type m, models )
								{
									std::string modelName = m.first;
									boost::shared_ptr <ObjectModel> objectModel = m.second;
									if (objectModel->findInstances(&segments)) {
										ShapeVector shapeVector = objectModel->getFoundObject();
										boost::shared_ptr<ObjectInstance> inst = boost::shared_ptr<ObjectInstance>(new ObjectInstance(objectModel, shapeVector));
										instances.push_back(inst);
										LOG(LINFO) << "ShapeRecognize::recognize(): instance of " + modelName
												+ " found: shapeVector.size() = " << shapeVector.size();
									}
								}
				}

	LOG(LDEBUG) << "ShapeRecognize::recognize(): found " << instances.size() << " instances.\n";
	return instances;
}

} // namespace CspObjectRecognize
} // namespace Processors
