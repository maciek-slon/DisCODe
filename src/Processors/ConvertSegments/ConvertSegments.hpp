/*!
 * \file ConvertSegments.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef CONVERTSEGMENTS_PROCESSOR_HPP_
#define CONVERTSEGMENTS_PROCESSOR_HPP_

#include <cv.h>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include "../GrayImageSegmentation/Segment.hpp"
#include "../GrayImageSegmentation/SegmentedImage.hpp"

/**
 * \defgroup ConvertSegments ConvertSegments
 * \ingroup Processors
 *
 * \brief Convert segments of segmented image to draw them in CvWindow.
 *
 *
 * \par Data streams:
 *
 * \streamin{in_segmented,Types::Segmentation::SegmentedImage}
 * Input segmented image.
 *
 * \streamout{out_img,cv::Mat}
 * Output image.
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready.
 *
 *
 * \par Event handlers:
 *
 * \handler{onSegmented}
 * New segmented image arrived.
 *
 *
 * \par Properties:
 *
 * \prop{showEdgeImage,bool,""}
 * If false, show whole segments.
 * If true, show only edges of segments.
 *
 * @{
 *
 * @}
 */

namespace Processors {
namespace ConvertSegments {

/*!
 * \brief ConvertSegments properties
 */
struct ConvertSegments_Props: public Base::Props
{
	/**
	 * If false, show whole segments. Show edges otherwise.
	 */
	bool showEdgeImage;
	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		showEdgeImage = pt.get <bool> ("showEdgeImage");
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put <bool> ("showEdgeImage", showEdgeImage);
	}

};

/*!
 * \class ConvertSegments_Processor
 * \brief ConvertSegments processor class.
 */
class ConvertSegments_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	ConvertSegments_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~ConvertSegments_Processor();

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

	/// Properties
	ConvertSegments_Props props;
private:
	/**
	 * Process segmented image and write to out_img.
	 */
	void onSegmented();

	/**
	 * Segmented image input data stream.
	 */
	Base::DataStreamIn <Types::Segmentation::SegmentedImage> in_segmented;

	/**
	 * On segmented image event handler.
	 */
	Base::EventHandler <ConvertSegments_Processor> h_onSegmented;

	/**
	 * Output image data stream.
	 */
	Base::DataStreamOut <cv::Mat> out_img;

	/**
	 * New image ready.
	 */
	Base::Event* newImage;

	/**
	 * Number of different segments' colors.
	 */
	static const int colorsSize = 16;

	/**
	 * Array of segments' colors.
	 */
	u_int8_t colors[colorsSize];
};

}//: namespace ConvertSegments
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("ConvertSegments", Processors::ConvertSegments::ConvertSegments_Processor, Common::Panel_Empty)

#endif /* CONVERTSEGMENTS_PROCESSOR_HPP_ */
