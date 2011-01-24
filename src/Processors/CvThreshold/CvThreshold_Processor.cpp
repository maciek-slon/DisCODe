/*!
 * \file CvThreshold_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvThreshold_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvThreshold {

CvThreshold_Processor::CvThreshold_Processor(const std::string & name) : Base::Component(name),
		m_type("type", CV_THRESH_BINARY, "combo"),
		m_thresh("thresh", 128, "range"),
		m_maxval("maxval", 255, "range")
{
	LOG(LTRACE) << "Hello CvThreshold_Processor\n";

	m_type.setToolTip("Thresholding type");
	m_type.addConstraint("BINARY");
	m_type.addConstraint("BINARY_INV");
	m_type.addConstraint("TRUNC");
	m_type.addConstraint("TOZERO");
	m_type.addConstraint("TOZERO_INV");

	m_thresh.setToolTip("Threshold level");
	m_thresh.addConstraint("0");
	m_thresh.addConstraint("255");

	m_maxval.setToolTip("Maximum value to use with BINARY and BINARY_INV thresholding types");
	m_maxval.addConstraint("0");
	m_maxval.addConstraint("255");

	registerProperty(m_type);
	registerProperty(m_thresh);
	registerProperty(m_maxval);
}

CvThreshold_Processor::~CvThreshold_Processor()
{
	LOG(LTRACE) << "Good bye CvThreshold_Processor\n";
}

bool CvThreshold_Processor::onInit()
{
	LOG(LTRACE) << "CvThreshold_Processor::initialize\n";

	h_onNewImage.setup(this, &CvThreshold_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);



	return true;
}

bool CvThreshold_Processor::onFinish()
{
	LOG(LTRACE) << "CvThreshold_Processor::finish\n";

	return true;
}

bool CvThreshold_Processor::onStep()
{
	LOG(LTRACE) << "CvThreshold_Processor::step\n";
	return true;
}

bool CvThreshold_Processor::onStop()
{
	return true;
}

bool CvThreshold_Processor::onStart()
{
	return true;
}

void CvThreshold_Processor::onNewImage()
{
	LOG(LTRACE) << "CvThreshold_Processor::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();
		LOG(LTRACE) << "Threshold " << m_thresh;
		cv::threshold(img, out, m_thresh, m_maxval, m_type);
		out_img.write(out);
		newImage->raise();
	} catch (...) {
		LOG(LERROR) << "CvThreshold::onNewImage failed\n";
	}
}

}//: namespace CvThreshold
}//: namespace Processors
