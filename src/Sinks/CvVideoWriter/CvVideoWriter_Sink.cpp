/*!
 * \file CvVideoWriter_Sink.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-15
 */

#include <memory>
#include <string>
#include <iostream>

#include "CvVideoWriter_Sink.hpp"
#include "Logger.hpp"
#include "Types/Drawable.hpp"

namespace Sinks {
namespace CvVideoWriter {

CvVideoWriter_Sink::CvVideoWriter_Sink(const std::string & name) : Base::Component(name) {
	LOG(LTRACE)<<"Hello CvVideoWriter_Sink\n";
}

CvVideoWriter_Sink::~CvVideoWriter_Sink() {
	LOG(LTRACE)<<"Good bye CvVideoWriter_Sink\n";
}

bool CvVideoWriter_Sink::onInit() {
	LOG(LTRACE) << "CvVideoWriter_Sink::initialize\n";

	h_onNewImage.setup(this, &CvVideoWriter_Sink::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);


	registerStream("in_img", &in_img);

	writer.open(props.filename, props.fourcc, props.fps, props.size);

	if (writer.isOpened())
		LOG(LTRACE) << "CameraOpenCV: device opened\n";
	else
		LOG(LWARNING) << "CameraOpenCV: device NOT opened!\n";

	return writer.isOpened();
}

bool CvVideoWriter_Sink::onFinish() {
	LOG(LTRACE) << "CvVideoWriter_Sink::finish\n";

	return true;
}

bool CvVideoWriter_Sink::onStep()
{
	LOG(LTRACE)<<"CvVideoWriter_Sink::step\n";
	return true;
}

bool CvVideoWriter_Sink::onStop()
{
	return true;
}

bool CvVideoWriter_Sink::onStart()
{
	return true;
}

void CvVideoWriter_Sink::onNewImage() {
	LOG(LTRACE)<<"CvVideoWriter_Sink::onNewImage\n";

	try {
		cv::Mat img = in_img.read().clone();

		writer << img;
	}
	catch(...) {
		LOG(LERROR) << "CvVideoWriter::onNewImage failed\n";
	}
}


}//: namespace CvVideoWriter
}//: namespace Sinks
