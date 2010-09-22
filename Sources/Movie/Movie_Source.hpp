/*!
 * \file Movie_Source.hpp
 * \brief Class responsible for retrieving images from movies
 * \author mstefanc
 * \date 2010-05-14
 */


#ifndef MOVIE_SOURCE_HPP_
#define MOVIE_SOURCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

/**
 * \defgroup Movie Movie
 * \ingroup Sources
 * \brief Movie reader, based on OpenCV.
 *
 *
 *
 * \par Data streams:
 *
 * \streamout{out_img,cv::Mat}
 * Output image
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready
 *
 *
 * \par Event handlers:
 *
 * \handler{onTrigger}
 * Trigger new frame
 *
 *
 * \par Properties:
 *
 * \prop{filename,string,""}
 * Name of movie file
 *
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/reading_and_writing_images_and_video.html#videocapture
 * @{
 *
 * @}
 */

namespace Sources {
namespace Movie {

using namespace cv;

/*!
 * \class Props
 * \brief Movie_Source properties
 */
struct Props : public Base::Props {

	std::string filename;
	bool triggered;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt) {
		filename  = pt.get("filename", "");
		triggered = pt.get("triggered", false);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt) {
		pt.put("filename", filename);
		pt.put("triggered", triggered);
	}
};

/*!
 * \class Movie_Source
 * \brief Class responsible for retrieving images from movies.
 */
class Movie_Source : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	Movie_Source(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~Movie_Source();

	/*!
	 * Return movie properties
	 */
	Base::Props * getProperties() {
		return &props;
	}

protected:
	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();



	/*!
	 * Event handler function.
	 */
	void onTrigger();

	/// Event handler.
	Base::EventHandler<Movie_Source> h_onTrigger;


	/// Event signaling that new image was retrieved.
	Base::Event * newImage;

	/// Output data stream
	Base::DataStreamOut<Mat> out_img;

	/// Capture device
	VideoCapture cap;

	/// Movie frame
	Mat frame;

	/// Movie properties
	Props props;

	bool trig;
};

}//: namespace Movie
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_SOURCE_COMPONENT("Movie", Sources::Movie::Movie_Source, Common::Panel_Empty)

#endif /* MOVIE_SOURCE_HPP_ */
