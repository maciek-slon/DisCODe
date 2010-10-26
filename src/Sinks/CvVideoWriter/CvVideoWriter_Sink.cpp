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
	LOG(TRACE)<<"Hello CvVideoWriter_Sink\n";
}

CvVideoWriter_Sink::~CvVideoWriter_Sink() {
	LOG(TRACE)<<"Good bye CvVideoWriter_Sink\n";
}

bool CvVideoWriter_Sink::onInit() {
	LOG(TRACE) << "CvVideoWriter_Sink::initialize\n";

	h_onNewImage.setup(this, &CvVideoWriter_Sink::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);


	registerStream("in_img", &in_img);

	writer.open(props.filename, props.fourcc, props.fps, props.size);

	if (writer.isOpened())
		LOG(TRACE) << "CameraOpenCV: device opened\n";
	else
		LOG(WARNING) << "CameraOpenCV: device NOT opened!\n";

	return writer.isOpened();
}

bool CvVideoWriter_Sink::onFinish() {
	LOG(TRACE) << "CvVideoWriter_Sink::finish\n";

	return true;
}

bool CvVideoWriter_Sink::onStep()
{
	LOG(TRACE)<<"CvVideoWriter_Sink::step\n";
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
	LOG(TRACE)<<"CvVideoWriter_Sink::onNewImage\n";

	try {
		cv::Mat img = in_img.read().clone();

		writer << img;
	}
	catch(...) {
		LOG(ERROR) << "CvVideoWriter::onNewImage failed\n";
	}
}


}//: namespace CvVideoWriter
}//: namespace Sinks
