/*!
 * \file GrayImageSegmentation.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef GRAYIMAGESEGMENTATION_PROCESSOR_HPP_
#define GRAYIMAGESEGMENTATION_PROCESSOR_HPP_

#include <cv.h>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include "SegmentExtractor.hpp"

/**
 * \defgroup GrayImageSegmentation GrayImageSegmentation
 * \ingroup Processors
 *
 * \brief Segmentation of grayscale image.
 *
 * \par Data streams:
 * \streamin{in_img,cv::Mat}
 * \streamout{out_segmented,Types::Segmentation::SegmentedImage}
 * \par Events:
 *
 * \event{onSegmented}
 * Image has been segmented.
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewImage}
 * New image arrived.
 *
 *
 * \par Properties:
 *
 * \prop{type,int,"minSegmentArea"}
 * Minimal segment size in pixels.
 *
 * \prop{type,double,"minVariance"}
 * Between 0 and 1, typically . Minimum variance in pixels value.
 * If variance is greater than this threshold for a segment, this segment is thresholded.
 *
 *
 * @{
 *
 * @}
 *
 */

namespace Processors {
namespace GrayImageSegmentation {

/*!
 * \brief GrayImageSegmentation properties
 */
struct GrayImageSegmentation_Props: public Base::Props
{
	/**
	 * Minimal segment area in pixels.
	 */
	int minSegmentArea;
	/**
	 * If segment variance is greater than this, segment is thresholded.
	 */
	double minVariance;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		minSegmentArea = pt.get <int> ("minSegmentArea");
		minVariance = pt.get <double> ("minVariance");
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("minSegmentArea", minSegmentArea);
		pt.put("minVariance", minVariance);
	}

};

/*!
 * \class GrayImageSegmentation_Processor
 * \brief GrayImageSegmentation processor class.
 */
class GrayImageSegmentation_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	GrayImageSegmentation_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~GrayImageSegmentation_Processor();

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
	GrayImageSegmentation_Props props;

	/**
	 * Process image.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler <GrayImageSegmentation_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/**
	 * Segmented image out data stream.
	 */
	Base::DataStreamOut <Types::Segmentation::SegmentedImage> out_segmented;

	/**
	 * Segmentation completed event.
	 */
	Base::Event * segmented;

	/**
	 * Segment extractor.
	 */
	SegmentExtractor segmentExtractor;
};

}//: namespace GrayImageSegmentation
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("GrayImageSegmentation", Processors::GrayImageSegmentation::GrayImageSegmentation_Processor, Common::Panel_Empty)

#endif /* GRAYIMAGESEGMENTATION_PROCESSOR_HPP_ */
