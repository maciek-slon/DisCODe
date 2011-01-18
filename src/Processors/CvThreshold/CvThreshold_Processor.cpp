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
		m_type_str("type", boost::bind(&CvThreshold_Processor::onTypeChanged, this, _1, _2), "binary", "combo"),
		m_thresh("thresh", 128, "range"),
		m_maxval("maxval", 255, "range")
{
	LOG(LTRACE) << "Hello CvThreshold_Processor\n";

	m_type_str.addConstraint("binary");
	m_type_str.addConstraint("inv_binary");
	m_type_str.addConstraint("trunc");
	m_type_str.addConstraint("zero");
	m_type_str.addConstraint("inv_zero");

	m_thresh.addConstraint("0");
	m_thresh.addConstraint("255");

	m_maxval.addConstraint("0");
	m_maxval.addConstraint("255");
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


	registerProperty(m_type_str);
	registerProperty(m_thresh);
	registerProperty(m_maxval);

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
		LOG(LTRACE) << "Threshold " << props.thresh;
		cv::threshold(img, out, m_thresh, m_maxval, m_type);
		out_img.write(out);
		newImage->raise();
	} catch (...) {
		LOG(LERROR) << "CvThreshold::onNewImage failed\n";
	}
}

void CvThreshold_Processor::onTypeChanged(const std::string & old_type, const std::string & new_type) {
	m_type = str2type(new_type);
}


}//: namespace CvThreshold
}//: namespace Processors
