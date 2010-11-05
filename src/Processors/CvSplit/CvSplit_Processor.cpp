/*!
 * \file CvSplit_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvSplit_Processor.hpp"
#include "Logger.hpp"

#include <boost/lexical_cast.hpp>

namespace Processors {
namespace CvSplit {

std::string int2str(int number) {
	return boost::lexical_cast<std::string>( number );
}

CvSplit_Processor::CvSplit_Processor(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello CvSplit_Processor\n";
}

CvSplit_Processor::~CvSplit_Processor()
{
	for (int i = 0; i < props.channels; ++i) {
		delete out_img[i];
	}
	LOG(LTRACE) << "Good bye CvSplit_Processor\n";
}

bool CvSplit_Processor::onInit()
{
	LOG(LTRACE) << "CvSplit_Processor::initialize\n";

	h_onNewImage.setup(this, &CvSplit_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	if (props.channels < 1) {
		LOG(LERROR) << "CvSplit_Processor::number of channels must be positive\n";
		return false;
	}

	mv.resize(props.channels);
	out_img.resize(props.channels);

	for (int i = 0; i < props.channels; ++i) {
		out_img[i] = new Base::DataStreamOut<cv::Mat>;
		registerStream("out_img_"+int2str(i), out_img[i]);
	}


	return true;
}

bool CvSplit_Processor::onFinish()
{
	LOG(LTRACE) << "CvSplit_Processor::finish\n";

	return true;
}

bool CvSplit_Processor::onStep()
{
	LOG(LTRACE) << "CvSplit_Processor::step\n";
	return true;
}

bool CvSplit_Processor::onStop()
{
	return true;
}

bool CvSplit_Processor::onStart()
{
	return true;
}

void CvSplit_Processor::onNewImage()
{
	LOG(LTRACE) << "CvSplit_Processor::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::split(img, mv);

		for (int i = 0; i < props.channels; ++i) {
			out_img[i]->write(mv[i]);
		}

		newImage->raise();
	} catch (...) {
		LOG(LERROR) << "CvSplit_Processor::onNewImage failed\n";
	}
}

}//: namespace CvSplit
}//: namespace Processors
