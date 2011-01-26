/**
 * \file CameraUniCap_Source.hpp
 * \brief Unicap-based camera source implementation
 * \date 2010-06-11
 * \author Konrad Banachowicz
 */

#include "CameraUniCap_Source.hpp"

#include "Logger.hpp"

#include <opencv/highgui.h>

namespace Sources {
namespace CameraUniCap {

#define MAX_DEVICES 64
#define MAX_FORMATS 64
#define MAX_PROPERTIES 64

CameraUniCap_Source::CameraUniCap_Source(const std::string & name) : Base::Component(name),
	m_buffer_type("buffer/type", boost::bind(&CameraUniCap_Source::onBufferTypeCahnged, this, _1, _2), "SYSTEM", "combo")
{
	m_buffer_type.addConstraint("SYSTEM");
	m_buffer_type.addConstraint("USER");
	m_buffer_type.setToolTip("Buffer type");
}

CameraUniCap_Source::~CameraUniCap_Source() {

}

bool CameraUniCap_Source::onInit() {

	unicap_device_t devices[MAX_DEVICES];
	unicap_format_t formats[MAX_FORMATS];
	unicap_property_t properties[MAX_PROPERTIES];

	int dev_count;
	int format_count;
	int property_count;

	bool device_found = false;

	unicap_status_t status = STATUS_SUCCESS;

	registerProperty(m_buffer_type);

	LOG(LINFO) << "CameraUniCap_Source::initialize()\n";
	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	/*
	 Get the all device found by the unicap library
	 */
	for (dev_count = 0; SUCCESS(status) && (dev_count < MAX_DEVICES); dev_count++) {
		status = unicap_enumerate_devices(NULL, &devices[dev_count], dev_count); // (1)
		if (SUCCESS(status))
			LOG(LINFO) << dev_count << " : "
					<< devices[dev_count].identifier << '\n';
		else
			break;
	}

	for (int i = 0; i < dev_count; i++) {
		if (props.device == devices[i].device) {
			device = devices[i];
			LOG(LINFO) << "device found\n";
			device_found = true;
			break;
		}
	}

	if (!device_found) {
		LOG(LERROR) << "Device not found: " << props.device << '\n';
		throw(Common::DisCODeException("Failed to open device"));
	}

	/*
	 Acquire a handle to this device
	 */
	if (!SUCCESS(unicap_open(&handle, &device))) {
		LOG(LERROR) << "Failed to open device: " << device.identifier
				<< '\n';
		throw(Common::DisCODeException("Failed to open device"));
	}

	/*
	 Get the video formats supported by the device
	 */
	status = STATUS_SUCCESS;

	for (format_count = 0; SUCCESS(status) && (format_count < MAX_FORMATS); format_count++) {
		status = unicap_enumerate_formats(handle, NULL, &formats[format_count],
				format_count);
		if (SUCCESS(status)) {
			LOG(LINFO) << format_count << ": "
					<< formats[format_count].identifier << '\n';
		} else {
			break;
		}
	}

	for (int i = 0; i < format_count; i++) {
		if (props.format == (char*) &formats[i].fourcc) {
			format = formats[i];
			LOG(LINFO) << "format found\n";
			break;
		}
	}

	for (int i = 0; i < format.size_count; i++) {
		if ((props.width == format.sizes[i].width) && (props.height
				== format.sizes[i].height)) {
			format.size = format.sizes[i];
			LOG(LINFO) << "size found\n";
			break;
		}
	}

	format.buffer_type = UNICAP_BUFFER_TYPE_SYSTEM;
	//format.buffer_type = UNICAP_BUFFER_TYPE_USER; // (1)

	/*
	 Set this video format
	 */
	if (!SUCCESS(unicap_set_format(handle, &format))) {
		LOG(LERROR) << "Failed to set video format\n";

	}

	buffer.data = new unsigned char[format.buffer_size]; // (2)
	buffer.buffer_size = format.buffer_size;

	status = STATUS_SUCCESS;

	for (property_count = 0; SUCCESS(status) && (property_count
			< MAX_PROPERTIES); property_count++) {
		status = unicap_enumerate_properties(handle, NULL,
				&properties[property_count], property_count);
		if (SUCCESS(status)) {
			unicap_get_property(handle, &properties[property_count]);
			if (properties[property_count].type == UNICAP_PROPERTY_TYPE_RANGE)
				LOG(LINFO) << "Property "
						<< properties[property_count].identifier
						<< ": Current = " << properties[property_count].value
						<< ", Range = ["
						<< properties[property_count].range.min << ".."
						<< properties[property_count].range.max << "]\n";

			if (std::string("Brightness")
					== properties[property_count].identifier) {
				if ((props.brightness <= 1.0) && (props.brightness >= 0.0)) {
					properties[property_count].value = props.brightness
							* properties[property_count].range.max;
					unicap_set_property(handle, &properties[property_count]);
				} else {
					LOG(LWARNING) << "Property "
							<< properties[property_count].identifier
							<< " out of range \n";
				}
			} else if (std::string("Contrast")
					== properties[property_count].identifier) {
				if ((props.contrast <= 1.0) && (props.contrast >= 0.0)) {
					properties[property_count].value = props.contrast
							* properties[property_count].range.max;
					unicap_set_property(handle, &properties[property_count]);
				} else {
					LOG(LWARNING) << "Property "
							<< properties[property_count].identifier
							<< " out of range \n";
				}
			} else if (std::string("Saturation")
					== properties[property_count].identifier) {
				if ((props.saturation <= 1.0) && (props.saturation >= 0.0)) {
					properties[property_count].value = props.saturation
							* properties[property_count].range.max;
					unicap_set_property(handle, &properties[property_count]);
				} else {
					LOG(LWARNING) << "Property "
							<< properties[property_count].identifier
							<< " out of range \n";
				}
			} else if (std::string("Hue")
					== properties[property_count].identifier) {
				if ((props.hue <= 1.0) && (props.hue >= 0.0)) {
					properties[property_count].value = props.hue
							* properties[property_count].range.max;
					unicap_set_property(handle, &properties[property_count]);
				} else {
					LOG(LWARNING) << "Property "
							<< properties[property_count].identifier
							<< " out of range \n";
				}
			} else

			if (std::string("video source")
					== properties[property_count].identifier) {
				LOG(LINFO) << "video sources : \n";
				for (int i = 0; i
						< properties[property_count].menu.menu_item_count; i++) {
					LOG(LINFO) << i << " : "
							<< properties[property_count].menu.menu_items[i]
							<< '\n';
					if (props.input
							== properties[property_count].menu.menu_items[i]) {
						strcpy(properties[property_count].menu_item,
								properties[property_count].menu.menu_items[i]);
						unicap_set_property(handle, &properties[property_count]);
					}
				}
			} else if (std::string("video norm")
					== properties[property_count].identifier) {
				LOG(LINFO) << "video norms : \n";
				for (int i = 0; i
						< properties[property_count].menu.menu_item_count; i++) {
					LOG(LINFO) << i << " : "
							<< properties[property_count].menu.menu_items[i]
							<< '\n';
					if (props.norm
							== properties[property_count].menu.menu_items[i]) {
						strcpy(properties[property_count].menu_item,
								properties[property_count].menu.menu_items[i]);
						unicap_set_property(handle, &properties[property_count]);
					}
				}
			}
		} else {
			break;
		}
	}

	unicap_register_callback(handle, UNICAP_EVENT_NEW_FRAME, (unicap_callback_t) new_frame_cb, this);

	LOG(LINFO)<<"CameraUniCap_Source::onInit() end\n";

	return true;
}

bool CameraUniCap_Source::onFinish() {
	LOG(LINFO) << "CameraUniCap_Source::finish()\n";

	/*
	 Close the device

	 This invalidates the handle
	 */
	if (!SUCCESS(unicap_close(handle))) {
		LOG(LERROR) << "Failed to close the device: " << device.identifier
				<< '\n';
	}

	return true;

}

bool CameraUniCap_Source::onStep() {
//	unicap_data_buffer_t * returned_buffer;
//
//	//LOG(LNOTICE) << "1";
//
//	if (!SUCCESS(unicap_wait_buffer(handle, &returned_buffer))) {
//		LOG(LERROR) << "Failed to wait for buffer!";
//		return false;
//	}
//
//	//LOG(LNOTICE) << "2";
//
//	cv::Mat frame = Mat(
//				format.size.height,
//				format.size.width,
//				CV_8UC3,
//				(void *) buffer.data
//			).clone();
//
//	//LOG(LNOTICE) << "3";
//
//	out_img.write(frame);
//	newImage->raise();
//
//	//LOG(LNOTICE) << "4";
//
//	if (!SUCCESS(unicap_queue_buffer(handle, returned_buffer))) {
//		LOG(LERROR) << "Failed to queue buffer!";
//		return false;
//	}
//
//	//LOG(LNOTICE) << "5";
//
	return true;
}

bool CameraUniCap_Source::onStart() {
	LOG(LINFO) << "CameraUniCap_Source::start()\n";
	/*
	 Start the capture process on the device
	 */
	if (!SUCCESS(unicap_start_capture(handle))) {
		LOG(LERROR) << "Failed to start capture on device: "	<< device.identifier << '\n';
		return false;
	}


	unicap_queue_buffer(handle, &buffer); // (4)


	return true;
}

bool CameraUniCap_Source::onStop() {
	LOG(LINFO) << "CameraUniCap_Source::stop()\n";
	/*
	 Stop the device
	 */
	if (!SUCCESS(unicap_stop_capture(handle))) {
		LOG(LERROR) << "Failed to stop capture on device: " << device.identifier << "\n";
		return false;
	}

	return true;
}

void CameraUniCap_Source::new_frame_cb(unicap_event_t event,
		unicap_handle_t handle, unicap_data_buffer_t *buffer, void *usr_data) {

	Mat
			frame =
					Mat(
							((CameraUniCap_Source*) (usr_data))->format.size.height,
							((CameraUniCap_Source*) (usr_data))->format.size.width,
							(std::string("GREY")
									== (char*) &((CameraUniCap_Source*) (usr_data))->format.fourcc) ? CV_8UC1
									: CV_8UC3, (void *) buffer->data).clone();

	LOG(LTRACE) << "Got new frame\n";

	((CameraUniCap_Source*) (usr_data))->out_img.write(frame);
	((CameraUniCap_Source*) (usr_data))->newImage->raise();


}

void CameraUniCap_Source::onBufferTypeCahnged(const std::string & old_type, const std::string & new_type) {

}


} //: CameraUniCap

} //: Sources
