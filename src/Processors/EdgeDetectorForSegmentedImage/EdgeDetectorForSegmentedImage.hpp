/*!
 * \file
 * \brief
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

namespace Processors {
namespace EdgeDetectorForSegmentedImage {

/*!
 * \brief EdgeDetectorForSegmentedImage properties
 */
struct EdgeDetectorForSegmentedImage_Props: public Base::Props
{

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{

	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{

	}

};

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

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties()
	{
		return &props;
	}

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
	/// Properties
	EdgeDetectorForSegmentedImage_Props props;

	void onSegmented();
	Base::DataStreamIn <Types::Segmentation::SegmentedImage> in_segmented;
	Base::EventHandler <EdgeDetectorForSegmentedImage_Processor> h_onSegmented;
	Base::DataStreamOut <Types::Segmentation::SegmentedImage> out_edgesDetected;
	Base::DataStreamOut <Types::DrawableContainer> out_contours;
	Base::Event* edgesDetected;
};

}//: namespace EdgeDetectorForSegmentedImage
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("EdgeDetectorForSegmentedImage", Processors::EdgeDetectorForSegmentedImage::EdgeDetectorForSegmentedImage_Processor, Common::Panel_Empty)

#endif /* EDGEDETECTORFORSEGMENTEDIMAGE_PROCESSOR_HPP_ */
