/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "LineSegments.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace LineSegments {

using Types::Segmentation::SegmentedImage;
using namespace Types;

LineSegments_Processor::LineSegments_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello LineSegments_Processor\n";
}

LineSegments_Processor::~LineSegments_Processor()
{
	LOG(LTRACE) << "Good bye LineSegments_Processor\n";
}

bool LineSegments_Processor::onInit()
{
	LOG(LTRACE) << "LineSegments_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!

	h_onEdgesDetected.setup(this, &LineSegments_Processor::onEdgesDetected);
	registerHandler("onEdgesDetected", &h_onEdgesDetected);

	registerStream("in_edgesDetected", &in_edgesDetected);
	registerStream("out_lineSegmentsEstimated", &out_lineSegmentsEstimated);
	registerStream("out_lineSegments", &out_lineSegments);
	lineSegmentsEstimated = registerEvent("lineSegmentsEstimated");
	return true;
}

bool LineSegments_Processor::onFinish()
{
	LOG(LTRACE) << "LineSegments_Processor::finish\n";

	return true;
}

bool LineSegments_Processor::onStep()
{
	LOG(LTRACE) << "LineSegments_Processor::step\n";
	return true;
}

bool LineSegments_Processor::onStop()
{
	return true;
}

bool LineSegments_Processor::onStart()
{
	return true;
}

void LineSegments_Processor::onEdgesDetected()
{
	LOG(LTRACE) << "LineSegments_Processor::onEdgesDetected()\n";
	SegmentedImage si = in_edgesDetected.read();

	DrawableContainer dc;
	for (size_t i = 0; i < si.segments.size(); ++i) {
		std::vector <Types::Line>* lines = si.segments[i].getLineSegments();
		for (size_t j = 0; j < lines->size(); ++j) {
			Line* line = new Line((*lines)[j]);
			dc.add(line);
			LOG(LTRACE) << "LineSegments_Processor::onEdgesDetected(): adding line segment\n";
		}
	}

	out_lineSegmentsEstimated.write(si);
	out_lineSegments.write(dc);
	lineSegmentsEstimated->raise();
}

}//: namespace LineSegments
}//: namespace Processors
