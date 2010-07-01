/*!
 * \file BlobExtractor_Processor.hpp
 * \brief Declaration of an example class, responsible for image processing.
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef BLOBEXTRACTOR_PROCESSOR_HPP_
#define BLOBEXTRACTOR_PROCESSOR_HPP_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"

#include <cv.h>
#include <cxtypes.h>

#include "Types/BlobResult.hpp"

namespace Processors {
namespace BlobExtractor {

/*!
 * \class BlobExtractor_Processor
 * \brief Example processor class.
 * \author tkornuta
 */
class BlobExtractor_Processor: public Base::Kernel
{
public:
	/*!
	 * Constructor.
	 */
	BlobExtractor_Processor();

	/*!
	 * Destructor
	 */
	virtual ~BlobExtractor_Processor();

	/*!
	 * Processor initialization
	 */
	bool initialize();

	/*!
	 * Release all resources
	 */
	bool finish();

	/*!
	 * Processes given frame.
	 */
	int step();

protected:
	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler for new image
	Base::EventHandler<BlobExtractor_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn<IplImage*> in_img;

	/// Event raised, when set of blobs is extracted
	Base::Event * newBlobs;

	/// Output data stream - list of detected blobs
	Base::DataStreamOut<Types::CBlobResult> out_blobs;

	/// Background color for blob extraction
	uchar bkg_color;
};

}//: namespace BlobExtractor
}//: namespace Processors


/*
 * Register processor kernel.
 */
REGISTER_PROCESSOR_KERNEL("BlobExtractor", Processors::BlobExtractor::BlobExtractor_Processor, Common::Panel_Empty)

#endif /* BLOBEXTRACTOR_PROCESSOR_HPP_ */

