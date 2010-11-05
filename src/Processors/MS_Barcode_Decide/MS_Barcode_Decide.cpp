/*!
 * \file MS_Barcode_Decide.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "MS_Barcode_Decide.hpp"
#include "Logger.hpp"

#include "Types/Rectangle.hpp"

namespace Processors {
namespace MS_Barcode {

MS_Barcode_Decide::MS_Barcode_Decide(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello MS_Barcode_Decide\n";
	blobs_ready = hue_ready = false;
}

MS_Barcode_Decide::~MS_Barcode_Decide()
{
	LOG(LTRACE) << "Good bye MS_Barcode_Decide\n";
}

bool MS_Barcode_Decide::onInit()
{
	LOG(LTRACE) << "MS_Barcode_Decide::initialize\n";

	h_onNewImage.setup(this, &MS_Barcode_Decide::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	h_onNewBlobs.setup(this, &MS_Barcode_Decide::onNewBlobs);
	registerHandler("onNewBlobs", &h_onNewBlobs);

	registerStream("in_blobs", &in_blobs);
	registerStream("in_hue", &in_hue);

	newImage = registerEvent("newImage");

	registerStream("out_signs", &out_signs);

	return true;
}

bool MS_Barcode_Decide::onFinish()
{
	LOG(LTRACE) << "MS_Barcode_Decide::finish\n";

	return true;
}

bool MS_Barcode_Decide::onStep()
{
	blobs_ready = hue_ready = false;

	try {
		int i;
		IplImage h = IplImage(hue_img);
		Types::Blobs::Blob *currentBlob;
		Types::DrawableContainer signs;

		// iterate through all found blobs
		for (i = 0; i < blobs.GetNumBlobs(); i++ )
		{
			currentBlob = blobs.GetBlob(i);

			// get mean color from area coverd by blob (from hue component)
			double me = currentBlob->Mean(&h);
			double st = currentBlob->StdDev(&h);

			// get blob bounding rectangle and ellipse
			//CvBox2D be = currentBlob->GetEllipse();
			cv::Rect bb = currentBlob->GetBoundingBox();

			signs.add(new Types::Rectangle(bb.x, bb.y, bb.width, bb.height));

		}

		out_signs.write(signs);

		newImage->raise();

		return true;
	} catch (...) {
		LOG(LERROR) << "MS_Sign_Decide::onNewImage failed\n";
		return false;
	}
}

bool MS_Barcode_Decide::onStop()
{
	return true;
}

bool MS_Barcode_Decide::onStart()
{
	return true;
}

void MS_Barcode_Decide::onNewImage()
{
	LOG(LTRACE) << "v::onNewImage\n";

	hue_ready = true;
	hue_img = in_hue.read();
	hue_img = hue_img.clone();
	if (blobs_ready && hue_ready)
		onStep();
}

void MS_Barcode_Decide::onNewBlobs()
{
	LOG(LTRACE) << "MS_Barcode_Decide::onNewBlobs\n";

	blobs_ready = true;
	blobs = in_blobs.read();
	//if (blobs_ready && hue_ready)
		onStep();
}


}//: namespace MS_Barcode
}//: namespace Processors
