/*!
 * \file MS_Sign_Decide.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "MS_Sign_Decide.hpp"
#include "Logger.hpp"

#include "Types/Ellipse.hpp"

namespace Processors {
namespace MS_Sign {

// OpenCV writes hue in range 0..180 instead of 0..360
#define H(x) (x>>1)

MS_Sign_Decide::MS_Sign_Decide(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello MS_Sign_Decide\n";
	blobs_ready = hue_ready = false;
}

MS_Sign_Decide::~MS_Sign_Decide()
{
	LOG(LTRACE) << "Good bye MS_Sign_Decide\n";
}

bool MS_Sign_Decide::onInit()
{
	LOG(LTRACE) << "MS_Sign_Decide::initialize\n";

	h_onNewImage.setup(this, &MS_Sign_Decide::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	h_onNewBlobs.setup(this, &MS_Sign_Decide::onNewBlobs);
	registerHandler("onNewBlobs", &h_onNewBlobs);

	registerStream("in_blobs", &in_blobs);
	registerStream("in_hue", &in_hue);

	newImage = registerEvent("newImage");

	registerStream("out_signs", &out_signs);

	return true;
}

bool MS_Sign_Decide::onFinish()
{
	LOG(LTRACE) << "MS_Sign_Decide::finish\n";

	return true;
}

bool MS_Sign_Decide::onStep()
{
	LOG(LTRACE) << "MS_Sign_Decide::step\n";

	blobs_ready = hue_ready = false;

	try {
		int id = 0;
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
			CvBox2D r2 = currentBlob->GetEllipse();

			// blob moments
			double m00, m10, m01, m11, m02, m20;
			double M11, M02, M20, M7;

			// probability that current blob is the one we need (roadsign) in range 0..255
			int prob;

			// ignore blobs that have only one color
			if (me < 130 || me > 155 || st < 20)
				continue;

			// calculate moments
			m00 = currentBlob->Moment(0,0);
			m01 = currentBlob->Moment(0,1);
			m10 = currentBlob->Moment(1,0);
			m11 = currentBlob->Moment(1,1);
			m02 = currentBlob->Moment(0,2);
			m20 = currentBlob->Moment(2,0);

			M11 = m11 - (m10*m01)/m00;
			M02 = m02 - (m01*m01)/m00;
			M20 = m20 - (m10*m10)/m00;

			// for circle it should be ~0.0063
			M7 = (M20*M02-M11*M11) / (m00*m00*m00*m00);

			// circle
			if (M7 < 0.007)
				prob = 255;
			// probably circle
			else if (M7 < 0.0085)
				prob = 128;
			// there is small chance that it's circular, but show them too
			else if (M7 < 0.011)
				prob = 10;
			else
			// not circle for sure
				continue;

			++id;

			signs.add(new Types::Ellipse(Point(r2.center.x, r2.center.y), Size(r2.size.width, r2.size.height), r2.angle));
		}

		out_signs.write(signs);

		newImage->raise();

		return true;
	} catch (...) {
		LOG(LERROR) << "MS_Sign_Decide::onNewImage failed\n";
		return false;
	}
}

bool MS_Sign_Decide::onStop()
{
	return true;
}

bool MS_Sign_Decide::onStart()
{
	return true;
}

void MS_Sign_Decide::onNewImage()
{
	LOG(LTRACE) << "MS_Sign_Decide::onNewImage\n";

	hue_ready = true;
	hue_img = in_hue.read();
	hue_img = hue_img.clone();
	if (blobs_ready && hue_ready)
		onStep();
}

void MS_Sign_Decide::onNewBlobs()
{
	LOG(LTRACE) << "MS_Sign_Decide::onNewBlobs\n";

	blobs_ready = true;
	blobs = in_blobs.read();
	if (blobs_ready && hue_ready)
		onStep();
}


}//: namespace MS_Sign
}//: namespace Processors
