/*
 * CameraUniCap.cpp
 *
 *  Created on: 2010-06-11
 *      Author: Konrad Banachowicz
 */

#include "Logger.hpp"

#include "CameraUniCap_Source.h"
#include <opencv/highgui.h>

namespace Sources {

namespace CameraUniCap {

#define MAX_DEVICES 64
#define MAX_FORMATS 64
#define MAX_PROPERTIES 64

CameraUniCap_Source::CameraUniCap_Source(const std::string & name) : Base::Component(name) {

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

	unicap_status_t status = STATUS_SUCCESS;

	LOG(INFO) << "CameraUniCap_Source::initialize()\n";
	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	/*
	 Get the all device found by the unicap library
	 */
	for (dev_count = 0; SUCCESS(status) && (dev_count < MAX_DEVICES); dev_count++) {
		status = unicap_enumerate_devices(NULL, &devices[dev_count], dev_count); // (1)
		if (SUCCESS(status))
			LOG(INFO) << dev_count << " : "
					<< devices[dev_count].identifier << '\n';
		else
			break;
	}

	for (int i = 0; i < dev_count; i++) {
		if (props.device == devices[i].device) {
			device = devices[i];
			LOG(INFO) << "device found\n";
			break;
		}
	}

	/*
	 Acquire a handle to this device
	 */
	if (!SUCCESS(unicap_open(&handle, &device))) {
		LOG(ERROR) << "Failed to open device: " << device.identifier
				<< '\n';
		throw(Common::FraDIAException("Failed to open device"));
	}

	/*
	 Get the video formats supported by the device
	 */
	status = STATUS_SUCCESS;

	for (format_count = 0; SUCCESS(status) && (format_count < MAX_FORMATS); format_count++) {
		status = unicap_enumerate_formats(handle, NULL, &formats[format_count],
				format_count);
		if (SUCCESS(status)) {
			LOG(INFO) << format_count << ": "
					<< formats[format_count].identifier << '\n';
		} else {
			break;
		}
	}

	for (int i = 0; i < format_count; i++) {
		if (props.format == (char*) &formats[i].fourcc) {
			format = formats[i];
			LOG(INFO) << "format found\n";
			break;
		}
	}

	for (int i = 0; i < format.size_count; i++) {
		if ((props.width == format.sizes[i].width) && (props.height
				== format.sizes[i].height)) {
			format.size = format.sizes[i];
			LOG(INFO) << "size found\n";
			break;
		}
	}

	format.buffer_type = UNICAP_BUFFER_TYPE_SYSTEM;

	/*
	 Set this video format
	 */
	if (!SUCCESS(unicap_set_format(handle, &format))) {
		LOG(ERROR) << "Failed to set video format\n";

	}

	status = STATUS_SUCCESS;

	for (property_count = 0; SUCCESS(status) && (property_count
			< MAX_PROPERTIES); property_count++) {
		status = unicap_enumerate_properties(handle, NULL,
				&properties[property_count], property_count);
		if (SUCCESS(status)) {
			unicap_get_property(handle, &properties[property_count]);
			if (properties[property_count].type == UNICAP_PROPERTY_TYPE_RANGE)
				LOG(INFO) << "Property "
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
					LOG(WARNING) << "Property "
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
					LOG(WARNING) << "Property "
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
					LOG(WARNING) << "Property "
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
					LOG(WARNING) << "Property "
							<< properties[property_count].identifier
							<< " out of range \n";
				}
			} else

			if (std::string("video source")
					== properties[property_count].identifier) {
				LOG(INFO) << "video sources : \n";
				for (int i = 0; i
						< properties[property_count].menu.menu_item_count; i++) {
					LOG(INFO) << i << " : "
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
				LOG(INFO) << "video norms : \n";
				for (int i = 0; i
						< properties[property_count].menu.menu_item_count; i++) {
					LOG(INFO) << i << " : "
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

	unicap_register_callback(handle, UNICAP_EVENT_NEW_FRAME,
			(unicap_callback_t) new_frame_cb, this);

	return true;
}

bool CameraUniCap_Source::onFinish() {
	LOG(INFO) << "CameraUniCap_Source::finish()\n";

	/*
	 Close the device

	 This invalidates the handle
	 */
	if (!SUCCESS(unicap_close(handle))) {
		LOG(ERROR) << "Failed to close the device: " << device.identifier
				<< '\n';
	}

	return true;

}

bool CameraUniCap_Source::onStep() {
	return true;
}

bool CameraUniCap_Source::onStart() {
	LOG(INFO) << "CameraUniCap_Source::start()\n";
	/*
	 Start the capture process on the device
	 */
	if (!SUCCESS(unicap_start_capture(handle))) {
		LOG(ERROR) << "Failed to start capture on device: "	<< device.identifier << '\n';
		return false;
	}
	return true;
}

bool CameraUniCap_Source::onStop() {
	LOG(INFO) << "CameraUniCap_Source::stop()\n";
	/*
	 Stop the device
	 */
	if (!SUCCESS(unicap_stop_capture(handle))) {
		LOG(ERROR) << "Failed to stop capture on device: " << device.identifier << "\n";
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

	LOG(TRACE) << "Got new frame\n";

	((CameraUniCap_Source*) (usr_data))->out_img.write(frame);
	((CameraUniCap_Source*) (usr_data))->newImage->raise();


}

}

}
