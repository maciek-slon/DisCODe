/*
 * CameraUniCap.h
 *
 *  Created on: 2010-06-11
 *      Author: konradb3
 */

#ifndef CAMERAUNICAP_H_
#define CAMERAUNICAP_H_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"

#include <unicap.h>
#include <cv.h>

namespace Sources {

namespace CameraUniCap {

using namespace cv;

/*!
 * \class CameraUniCapProps
 * \brief CameraUniCap_Source properties
 */
struct CameraUniCapProps : public Base::Props {

	std::string device;
	std::string format;

	int width;
	int height;

	/*!
	 * \copydoc Common::Props::load
	 */
	void load(const ptree & pt) {
		device = pt.get("dev.device", "video0");
		format = pt.get("dev.format", "BGR3");
		width = pt.get("dev.width", 640);
		height = pt.get("dev.height", 480);
	}

	/*!
	 * \copydoc Common::Props::save
	 */
	void save(ptree & pt) {
		pt.put("dev.device", device);
		pt.put("dev.format", format);
		pt.put("dev.width", width);
		pt.put("dev.height", height);
	}
};

class CameraUniCap_Source: public Base::Kernel {
public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	CameraUniCap_Source();

	/*!
	 * Destructor.
	 */
	virtual ~CameraUniCap_Source();

	/*!
	 * Connects source to given device.
	 */
	void initialize();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	void finish();

	/*!
	 * Retrieves data from device.
	 */
	int step();

	Base::Props * getProperties() {
		return &props;
	}

protected:
	/// Event signaling that new image was retrieved.
	Base::Event * newImage;

	/// Output data stream
	Base::DataStreamOut<Mat> out_img;

	/// Frame
	Mat frame;

	unicap_handle_t handle;
	unicap_device_t device;
	unicap_format_t format;
	unicap_data_buffer_t buffer;

	static void new_frame_cb(unicap_event_t event, unicap_handle_t handle,
			unicap_data_buffer_t *buffer, void *usr_data);

	/// Movie properties
	CameraUniCapProps props;
};

}
}

/*
 * Register source kernel.
 */
REGISTER_SOURCE_KERNEL("CameraUniCap", Sources::CameraUniCap::CameraUniCap_Source, Common::Panel_Empty)
#endif /* CAMERAUNICAP_H_ */
