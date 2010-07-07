/*!
 * \file CameraOpenCV_Source.hpp
 * \brief Class responsible for retrieving images from camera
 * \author mstefanc
 * \date 2010-06-10
 */


#ifndef CAMERAOPENCV_SOURCE_HPP_
#define CAMERAOPENCV_SOURCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"

#include <cv.h>
#include <highgui.h>

namespace Sources {
namespace CameraOpenCV {

using namespace cv;

/*!
 * \class CameraOpenCV_Source
 * \brief Class responsible for retrieving images from movies.
 */
class CameraOpenCV_Source : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	CameraOpenCV_Source();

	/*!
	 * Destructor.
	 */
	virtual ~CameraOpenCV_Source();

	/*!
	 * Connects source to given device.
	 */
	bool initialize();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool finish();

	/*!
	 * Retrieves data from device.
	 */
	int step();

protected:
	/// Event signaling that new image was retrieved.
	Base::Event * newImage;

	/// Output data stream
	Base::DataStreamOut<Mat> out_img;

	/// Capture device
	VideoCapture cap;

	/// Movie frame
	Mat frame;
};

}//: namespace CameraOpenCV
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_SOURCE_COMPONENT("CameraOpenCV", Sources::CameraOpenCV::CameraOpenCV_Source, Common::Panel_Empty)

#endif /* CAMERAOPENCV_SOURCE_HPP_ */
