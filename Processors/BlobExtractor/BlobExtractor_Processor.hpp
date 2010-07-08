/*!
 * \file BlobExtractor_Processor.hpp
 * \brief Declaration of an example class, responsible for image processing.
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef BLOBEXTRACTOR_PROCESSOR_HPP_
#define BLOBEXTRACTOR_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"

#include <cv.h>
#include <cxtypes.h>

#include "BlobResult.hpp"

namespace Processors {
namespace BlobExtractor {

/*!
 * \class BlobExtractor_Processor
 * \brief Example processor class.
 * \author tkornuta
 */
class BlobExtractor_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	BlobExtractor_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~BlobExtractor_Processor();

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

	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler for new image
	Base::EventHandler<BlobExtractor_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn<cv::Mat> in_img;

	/// Event raised, when new image with blobs is ready
	Base::Event * newImage;

	/// Output data stream - image with drawn blobs
	Base::DataStreamOut<cv::Mat> out_img;

	/// Event raised, when set of blobs is extracted
	Base::Event * newBlobs;

	/// Output data stream - list of detected blobs
	Base::DataStreamOut<Types::Blobs::BlobResult> out_blobs;

	/// Background color for blob extraction
	uchar bkg_color;
};

}//: namespace BlobExtractor
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("BlobExtractor", Processors::BlobExtractor::BlobExtractor_Processor, Common::Panel_Empty)

#endif /* BLOBEXTRACTOR_PROCESSOR_HPP_ */

