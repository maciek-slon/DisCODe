/*!
 * \file Sequence.hpp
 * \brief Class responsible for retrieving images from image sequences - class declaration.
 */


#ifndef CAMERAGIGE_HPP_
#define CAMERAGIGE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <vector>
#include <string>

#include <cv.h>

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Winsock2.h>
#endif

//#if defined(_LINUX) || defined(_QNX) || defined(_OSX)
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/times.h>
#include <arpa/inet.h>
//#endif

#define _LINUX
#define _x64

#include <PvApi.h>

/**
 * \defgroup CameraGigE CameraGigE
 * \ingroup Sources
 *
 * \brief ATV GigE camera driver.
 *
 * Provide access to GieE Vision cameras.
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
 * Trigger new frame in SoftTrigger mode
 *
 *
 * \par Properties:
 *
 * \prop{Adress,string,"192.168.1.2"}
 * IP address of camera.
 * \prop{UID,int,"0"}
 * UID of camera.
 *
 * \prop{Exposure.Mode,string,"Auto"}
 * Control exposure mode, available modes : Manual, Auto, AutoOnce, External.
 * \prop{Exposure.Value,double,0.01}
 * Exposure time in seconds used in Manual mode.
 *
 * \prop{Gain.Mode,string,"Auto"}
 * Control gain mode, available modes : Manual, Auto, AutoOnce.
 * \prop{Gain.Value,int,0}
 * Sensor gain in dB used in Manual mode.
 *
 * \prop{Whitebal.Mode,string,"Auto"}
 * Control White Balance mode, available modes : Manual, Auto, AutoOnce.
 * \prop{Whitebal.ValueRed,int,50}
 * Red gain expressed as a percentage of the camera default setting.
 * \prop{Whitebal.ValueBlue,int,50}
 * Blue gain expressed as a percentage of the camera default setting.
 *
 * \prop{ImageFormat.PixelFormat,string,"Bgr24"}
 * Pixel format, available formats : Mono8, Bgr24.
 *
 * \prop{ImageFormat.MirrorX,bool,false}
 * Enable horizontal mirroring of the image.
 * \prop{ImageFormat.ROI.Height,int,480}
 * The vertical size of the rectangle that defines the ROI.
 * \prop{ImageFormat.ROI.Width,int,640}
 * The horizontal size of the rectangle that defines the ROI.
 * \prop{ImageFormat.ROI.RegionX,int,0}
 * The X position of the top-left corner of the ROI.
 * \prop{ImageFormat.ROI.RegionY,int,0}
 * The Y position of the top-left corner of the ROI.
 *
 * \prop{ImageMode.BinningX,int,0}
 * The horizontal binning factor.
 * Binning is the summing of charge of adjacent pixels on a sensor, to give a lower resolution but more sensitive image.
 * \prop{ImageMode.BinningY,int,0}
 * The vertical binning factor.
 *
 * @{
 *
 * @}
 */

namespace Sources {
namespace CameraGigE {

struct Props : public Base::Props {
	// Device
	std::string address;
	uint32_t uid;

	// Controls
	/// Exposure
	std::string 	exposureMode;
	double 			exposureValue;
	/// Gain
	std::string		gainMode;
	int				gainValue;
	/// Iris

	/// WhiteBalance
	std::string		whitebalMode;
	int 			whitebalValueRed;
	int				whitebalValueBlue;

	// ImageFormat
	bool			mirrorX;
	std::string		pixelFormat;
	///ROI
	int 			height;
	int 			width;
	int				regionX;
	int				regionY;

	// ImageMode
	int				binningX;
	int				binningY;

	void load(const ptree & pt) {
		address = pt.get("Address", "");
		uid = pt.get("UID", 0);

		exposureMode = pt.get("Controls.Exposure.Mode", "Auto");
		exposureValue = pt.get("Controls.Exposure.Value", 0.05);

		gainMode = pt.get("Controls.Gain.Mode", "Auto");
		gainValue = pt.get("Controls.Gain.Value", 0);

		whitebalMode = pt.get("Controls.WhiteBalance.Mode", "Auto");
		whitebalValueRed = pt.get("Controls.WhiteBalance.ValueRed", 50);
		whitebalValueBlue = pt.get("Controls.WhiteBalance.ValueBlue", 50);

		mirrorX = pt.get("ImageFormat.MirrorX", false);
		pixelFormat = pt.get("ImageFormat.PixelFormat", "Bgr24");
		height = pt.get("ImageFormat.ROI.Height", 480);
		width = pt.get("ImageFormat.ROI.Width", 640);
		regionX = pt.get("ImageFormat.ROI.RegionX", 0);
		regionY = pt.get("ImageFormat.ROI.RegionY", 0);

		binningX = pt.get("ImageMode.BinningX", 0);
		binningY = pt.get("ImageMode.BinningY", 0);

	}

	void save(ptree & pt) {
		pt.put("Address", address);
		pt.put("UID", uid);

		pt.put("Controls.Exposure.Mode", exposureMode);
		pt.put("Controls.Exposure.Value", exposureValue);

		pt.put("Controls.Gain.Mode", gainMode);
		pt.put("Controls.Gain.Value", gainValue);

		pt.put("Controls.WhiteBalance.Mode", whitebalMode);
		pt.put("Controls.WhiteBalance.ValueRed", whitebalValueRed);
		pt.put("Controls.WhiteBalance.ValueBlue", whitebalValueBlue);

		pt.put("ImageFormat.MirrorX", mirrorX);
		pt.put("ImageFormat.PixelFormat", pixelFormat);
		pt.put("ImageFormat.ROI.Height", height);
		pt.put("ImageFormat.ROI.Width", width);
		pt.put("ImageFormat.ROI.RegionX", regionX);
		pt.put("ImageFormat.ROI.RegionY", regionY);

		pt.put("ImageMode.BinningX", binningX);
		pt.put("ImageMode.BinningY", binningY);
	}
};

/*!
 * \class Sequence
 * \brief Class responsible for retrieving images from image sequences.
 */
class CameraGigE : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	CameraGigE(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~CameraGigE();

	/*!
	 * Return sequence properties
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


	/// Event signaling that new image was retrieved.
	Base::Event * newImage;

	/// Sequence has ended
	Base::Event * endOfSequence;

	/// Output data stream
	Base::DataStreamOut<cv::Mat> out_img;


	/*!
	 * Event handler function.
	 */
	void onTrigger();

	/// Event handler.
	Base::EventHandler<CameraGigE> h_onTrigger;

private:
	/// Camera handle
	tPvHandle 	cHandle;
	tPvFrame	frame;

	/// current frame
	cv::Mat img;

	/// sequence properties
	Props props;

};

}//: namespace Sequence
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_SOURCE_COMPONENT("CameraGigE", Sources::CameraGigE::CameraGigE, Common::Panel_Empty)

#endif /* CAMERAGIGE_HPP_ */
