/*!
 * \file EdgeDetectorForSegmentedImage.hpp
 * \brief
 * \author mbotyn
 * \date 2011-01-22
 */

#ifndef EDGEDETECTORFORSEGMENTEDIMAGE_PROCESSOR_HPP_
#define EDGEDETECTORFORSEGMENTEDIMAGE_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"
#include "DrawableContainer.hpp"
#include "Contour.hpp"

#include "../GrayImageSegmentation/SegmentedImage.hpp"


/**
 * \defgroup EdgeDetectorForSegmentedImage EdgeDetectorForSegmentedImage
 * \ingroup Processors
 *
 * \brief Detects edges in segmented image.
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_segmented,Types::Segmentation::SegmentedImage}
 * Segmented image data stream.
 *
 * \streamout{out_edgesDetected,Types::Segmentation::SegmentedImage}
 * Segmented image with detected edges.
 *
 * \streamout{out_contours,Types::DrawableContainer}
 * Edges to draw in CvWindow.
 *
 *
 * \par Events:
 *
 * \event{edgesDetected}
 * Edges have been detected.
 *
 *
 * \par Event handlers:
 *
 * \handler{onSegmented}
 * New segmented image is ready on input data stream.
 *
 * @{
 *
 * @}
 */

namespace Processors {
namespace EdgeDetectorForSegmentedImage {

/*!
 * \class EdgeDetectorForSegmentedImage_Processor
 * \brief EdgeDetectorForSegmentedImage processor class.
 */
class EdgeDetectorForSegmentedImage_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	EdgeDetectorForSegmentedImage_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~EdgeDetectorForSegmentedImage_Processor();

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
	 * Process segmented image.
	 */
	void onSegmented();

	/**
	 * Segmented image data stream.
	 */
	Base::DataStreamIn <Types::Segmentation::SegmentedImage> in_segmented;

	/**
	 * Event handler for segmented image.
	 */
	Base::EventHandler <EdgeDetectorForSegmentedImage_Processor> h_onSegmented;

	/**
	 * Segmented image with detected edges.
	 */
	Base::DataStreamOut <Types::Segmentation::SegmentedImage> out_edgesDetected;

	/**
	 * Detected contours.
	 */
	Base::DataStreamOut <Types::DrawableContainer> out_contours;

	/**
	 * Edges detected event.
	 */
	Base::Event* edgesDetected;
};

}//: namespace EdgeDetectorForSegmentedImage
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("EdgeDetectorForSegmentedImage", Processors::EdgeDetectorForSegmentedImage::EdgeDetectorForSegmentedImage_Processor, Common::Panel_Empty)

#endif /* EDGEDETECTORFORSEGMENTEDIMAGE_PROCESSOR_HPP_ */
