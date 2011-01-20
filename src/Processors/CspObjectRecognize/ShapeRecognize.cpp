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

void ShapeRecognize::setModels(std::vector <boost::shared_ptr <ObjectModel> > models)
{
	this->models = models;
}

void ShapeRecognize::recognize(Types::Segmentation::SegmentedImage& si)
{
	ShapeSegments segments;
	BOOST_FOREACH(Segment& s, si.segments)
				{
					BOOST_FOREACH(Types::Line& line, *s.getLineSegments())
								{
									boost::shared_ptr <AbstractShape> shape =
											boost::shared_ptr <AbstractShape>(new LineSegment(line));
									segments.push_back(shape);
								}
				}

	LOG(LFATAL) << "ShapeRecognize::recognize()\n";
	BOOST_FOREACH(boost::shared_ptr<ObjectModel> m, models)
				{
					m->findInstances(&segments);
				}
}

} // namespace CspObjectRecognize
} // namespace Processors
