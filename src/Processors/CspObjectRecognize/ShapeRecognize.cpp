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

void ShapeRecognize::recognize(Types::Segmentation::SegmentedImage& si)
{
	BOOST_FOREACH(Segment& s, si.segments)
				{
					ShapeSegments segments;
					BOOST_FOREACH(Types::Line& line, *s.getLineSegments())
								{
									boost::shared_ptr <AbstractShape> shape =
											boost::shared_ptr <AbstractShape>(new LineSegment(line));
									segments.push_back(shape);
								}
					LOG(LFATAL) << "ShapeRecognize::recognize(): trying to recognize segoment: segments.size() = "
							<< segments.size();
					//BOOST_FOREACH(boost::shared_ptr<ObjectModel> m, models)
					BOOST_FOREACH(ModelsMap::value_type m, models )
								{
									string modelName = m.first;
									boost::shared_ptr <ObjectModel> objectModel = m.second;
									if (objectModel->findInstances(&segments)) {
										ShapeVector shapeVector = objectModel->getFoundObject();
										LOG(LFATAL) << "ShapeRecognize::recognize(): instance of " + modelName
												+ " found: shapeVector.size() = " << shapeVector.size();
									}
								}
				}
}

} // namespace CspObjectRecognize
} // namespace Processors
