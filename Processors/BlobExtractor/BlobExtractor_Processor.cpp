/*!
 * \file BlobExtractor_Processor.cpp
 * \brief Declaration of an example class,
 * responsible for image processing.
 * - methods definitions
 * \author tkornuta
 * \date 11.03.2008
 */

#include <memory>
#include <string>
#include <iostream>

#include "BlobExtractor_Processor.hpp"
#include "ComponentLabeling.hpp"

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Processors {
namespace BlobExtractor {

BlobExtractor_Processor::BlobExtractor_Processor() {
	cout<<"Hello BlobExtractor_Processor\n";

	initialize();
}

BlobExtractor_Processor::~BlobExtractor_Processor() {
	finish();

	cout<<"Good bye BlobExtractor_Processor\n";
}

bool BlobExtractor_Processor::initialize() {
	std::cout << "BlobExtractor_Processor::initialize\n";

	newBlobs = registerEvent("newBlobs");

	h_onNewImage.setup(this, &BlobExtractor_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);
	registerStream("out_blobs", &out_blobs);

	bkg_color = 0;

	return true;
}

bool BlobExtractor_Processor::finish() {
	std::cout << "BlobExtractor_Processor::finish\n";

	return true;
}

int BlobExtractor_Processor::step()
{
	cout<<"BlobExtractor_Processor::step\n";
	return 0;
}

void BlobExtractor_Processor::onNewImage() {
	cout << "BlobExtractor_Processor::onNewImage() called!\n";

	IplImage * img = in_img.read();

	Types::Blobs::Blob_vector res;
	bool success;

	try
	{
		success = ComponentLabeling( img, NULL, bkg_color, res );
	}
	catch(...)
	{
		success = false;
	}

	if( !success ) {
		std::cout << "Blob find error\n";
	} else {
		Types::Blobs::BlobResult result(res);

		out_blobs.write(result);

		newBlobs->raise();
	}
}

}//: namespace BlobExtractor
}//: namespace Processors
