/**
 * \file CameraUniCap_Source.hpp
 * \brief Unicap-based camera source definition
 * \date 2010-06-11
 * \author Konrad Banachowicz
 */

#ifndef CAMERAUNICAP_HPP_
#define CAMERAUNICAP_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include <unicap.h>
#include <opencv/cv.h>

/**
 * \defgroup CameraUniCap CameraUniCap
 * \ingroup Sources
 * \brief Source retrieving frames from cameras, based on UniCap driver.
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
 * None at the moment
 *
 *
 * \par Properties:
 *
 * \prop{dev.device,string,"/dev/video0"}
 * Device to grab frames from
 * \prop{dev.input,string,"Composite1"}
 * Input (when device is framegrabber)
 * \prop{dev.norm,string,"PAL-BG"}
 * Video standard
 * \prop{dev.formar,string,"BGR3"}
 * Pixel format
 * \prop{dev.width,int,640}
 * Frame width
 * \prop{dev.height,int,480}
 * Frame height
 *
 * All below properties are in range 0.0-1.0.
 * \prop{image.brightness,double,0.5}
 * Image brightness
 * \prop{image.contrast,double,0.5}
 * Image contrast
 * \prop{image.saturation,double,0.5}
 * Image saturation
 * \prop{image.hue,double,0.5}
 * Image hue
 *
 *
 * \see http://unicap-imaging.org/
 * @{
 *
 * @}
 */

namespace Sources {
namespace CameraUniCap {

using namespace cv;

/*!
 * \class CameraUniCapProps
 * \brief CameraUniCap_Source properties
 */
struct CameraUniCapProps: public Base::Props
{

	// Device properties
	std::string device;
	std::string format;
	std::string input;
	std::string norm;

	int width;
	int height;

	// Image properties

	double brightness;
	double contrast;
	double saturation;
	double hue;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		device = pt.get("dev.device", "/dev/video0");
		input = pt.get("dev.input", "Composite1");
		norm = pt.get("dev.norm", "PAL-BG");
		format = pt.get("dev.format", "BGR3");
		width = pt.get("dev.width", 640);
		height = pt.get("dev.height", 480);

		brightness = pt.get("image.brightness", 0.5);
		contrast = pt.get("image.contrast", 0.5);
		saturation = pt.get("image.saturation", 0.5);
		hue = pt.get("image.hue", 0.5);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("dev.device", device);
		pt.put("dev.input", input);
		pt.put("dev.norm", norm);
		pt.put("dev.format", format);
		pt.put("dev.width", width);
		pt.put("dev.height", height);

		pt.put("image.brightness", brightness);
		pt.put("image.contrast", contrast);
		pt.put("image.saturation", saturation);
		pt.put("image.hue", hue);
	}
};

/*!
 * \brief Unicap-based camera source.
 */
class CameraUniCap_Source: public Base::Component
{
public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	CameraUniCap_Source(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~CameraUniCap_Source();

	Base::Props * getProperties()
	{
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

	/// Output data stream
	Base::DataStreamOut <Mat> out_img;

	/// Frame
	Mat frame;

	unicap_handle_t handle;
	unicap_device_t device;
	unicap_format_t format;
	unicap_data_buffer_t buffer;

	/// Movie properties
	CameraUniCapProps props;

private:
	static void	new_frame_cb(unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t *buffer, void *usr_data);

	Base::Property<std::string> m_buffer_type;
	void onBufferTypeCahnged(const std::string & old_type, const std::string & new_type);
};

}
}

/*
 * Register source component.
 */
REGISTER_SOURCE_COMPONENT("CameraUniCap", Sources::CameraUniCap::CameraUniCap_Source, Common::Panel_Empty)

#endif /* CAMERAUNICAP_HPP_ */
