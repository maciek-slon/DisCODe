/*!
 * \file CvWindow_Sink.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-15
 */

#include <memory>
#include <string>
#include <iostream>

#include "CvWindow_Sink.hpp"
#include "Logger.hpp"
#include "Types/Drawable.hpp"

#include <boost/bind.hpp>

namespace Sinks {
namespace CvWindow {

CvWindow_Sink::CvWindow_Sink(const std::string & name) : Base::Component(name) {
	LOG(LTRACE)<<"Hello CvWindow_Sink\n";
}

CvWindow_Sink::~CvWindow_Sink() {
	LOG(LTRACE)<<"Good bye CvWindow_Sink\n";
}

bool CvWindow_Sink::onInit() {
	LOG(LTRACE) << "CvWindow_Sink::initialize\n";

	Base::EventHandler2 * hand;
	for (int i = 0; i < props.count; ++i) {
		char id = '0'+i;
		hand = new Base::EventHandler2;
		hand->setup(boost::bind(&CvWindow_Sink::onNewImageN, this, i));
		handlers.push_back(hand);
		registerHandler(std::string("onNewImage")+id, hand);

		in_img.push_back(new Base::DataStreamIn<cv::Mat>);
		registerStream(std::string("in_img")+id, in_img[i]);

		in_draw.push_back(new Base::DataStreamInPtr<Types::Drawable, Base::DataStreamBuffer::Newest>);
		registerStream(std::string("in_draw")+id, in_draw[i]);

		//cv::namedWindow(props.title + id);
	}
	//waitKey( 1000 );

	// register aliases for first handler and streams
	registerHandler("onNewImage", handlers[0]);
	registerStream("in_img", in_img[0]);
	registerStream("in_draw", in_draw[0]);

	img.resize(props.count);
	to_draw.resize(props.count);

	return true;
}

bool CvWindow_Sink::onFinish() {
	LOG(LTRACE) << "CvWindow_Sink::finish\n";

	return true;
}

bool CvWindow_Sink::onStep()
{
	LOG(LTRACE)<<"CvWindow_Sink::step\n";

	try {
		for (int i = 0; i < props.count; ++i) {
			char id = '0' + i;

			if (img[i].empty()) {
				LOG(LWARNING) << name() << ": image " << i << " empty";
			}

			// Refresh image.
			imshow( props.title + id, img[i] );
		}

		waitKey( 10 );
	}
	catch(...) {
		LOG(LERROR) << "CvWindow::onNewImage failed\n";
	}

	return true;
}

bool CvWindow_Sink::onStop()
{
	return true;
}

bool CvWindow_Sink::onStart()
{
	return true;
}

void CvWindow_Sink::onNewImage() {
	LOG(LTRACE)<<"CvWindow_Sink::onNewImage\n";
}

void CvWindow_Sink::onNewImageN(int n) {
	LOG(LTRACE) << name() << "::onNewImage(" << n << ")";

	try {
		img[n] = in_img[n]->read().clone();

		if (!in_draw[n]->empty()) {
			to_draw[n] = in_draw[n]->read();
		}

		if (to_draw[n]) {
			to_draw[n]->draw(img[n], CV_RGB(255,0,255));
			to_draw[n] = boost::shared_ptr<Types::Drawable>();
		}

		// Display image.
		onStep();
	}
	catch(...) {
		LOG(LERROR) << "CvWindow::onNewImage failed\n";
	}
}


}//: namespace CvWindow
}//: namespace Sinks
