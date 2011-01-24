/*!
 * \file LineSegments.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef LINESEGMENTS_PROCESSOR_HPP_
#define LINESEGMENTS_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include "../GrayImageSegmentation/SegmentedImage.hpp"
#include "DrawableContainer.hpp"


/**
 * \defgroup LineSegments LineSegments
 * \ingroup Processors
 *
 * \brief Estimate line segments from segmented image with edges detected.
 *
 *
 * \par Data streams:
 *
 * \streamin{in_edgesDetected,Types::Segmentation::SegmentedImage}
 * Input segmented image with edges detected.
 *
 * \streamout{out_lineSegmentsEstimated,Types::Segmentation::SegmentedImage}
 * Line segments estimated.
 *
 * \streamout{out_lineSegments,Types::DrawableContainer}
 * Line segments ready to draw.
 *
 *
 * \par Events:
 *
 * \event{lineSegmentsEstimated}
 * Line segments estimated event.
 *
 *
 * \par Event handlers:
 *
 * \handler{onEdgesDetected}
 * New segmented image with edges detected arrived.
 *
 *
 * @{
 *
 * @}
 */


namespace Processors {
namespace LineSegments {

/*!
 * \class LineSegments_Processor
 * \brief LineSegments processor class.
 */
class LineSegments_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	LineSegments_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~LineSegments_Processor();
protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

private:
	/**
	 * Segmented image with detected edges.
	 */
	void onEdgesDetected();

	/**
	 * Event handler.
	 */
	Base::EventHandler <LineSegments_Processor> h_onEdgesDetected;

	/**
	 * Segmented image input data stream.
	 */
	Base::DataStreamIn <Types::Segmentation::SegmentedImage> in_edgesDetected;

	/**
	 * Segmented image with line segments estimated.
	 */
	Base::DataStreamOut <Types::Segmentation::SegmentedImage> out_lineSegmentsEstimated;

	/**
	 * Line segments to draw in CvWindow.
	 */
	Base::DataStreamOut <Types::DrawableContainer> out_lineSegments;

	/**
	 * Line segments estimated event.
	 */
	Base::Event* lineSegmentsEstimated;
};

}//: namespace LineSegments
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("LineSegments", Processors::LineSegments::LineSegments_Processor, Common::Panel_Empty)

#endif /* LINESEGMENTS_PROCESSOR_HPP_ */
