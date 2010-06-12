/*
 * CameraUniCap.cpp
 *
 *  Created on: 2010-06-11
 *      Author: konradb3
 */

#include "Logger.hpp"

#include "CameraUniCap_Source.h"
#include <highgui.h>

namespace Sources {

namespace CameraUniCap {

#define MAX_DEVICES 64
#define MAX_FORMATS 64
CameraUniCap_Source::CameraUniCap_Source() {

}

CameraUniCap_Source::~CameraUniCap_Source() {

}

void CameraUniCap_Source::initialize() {

	unicap_device_t devices[MAX_DEVICES];
	unicap_format_t formats[MAX_FORMATS];

	int dev_count;
	int format_count;
	unicap_status_t status = STATUS_SUCCESS;

	LOG(INFO) << "CameraOpenCV_Source::initialize()\n";
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
		status = unicap_enumerate_formats(handle, NULL, &formats[format_count], // (1)
				format_count);
		if (SUCCESS(status)) {
			LOG(INFO) << format_count << ": " << formats[format_count].identifier << '\n';
		} else {
			break;
		}
	}

	for (int i = 0; i < format_count; i++) {
		if (props.format == (char*)&formats[i].fourcc ) {
			format = formats[i];
			LOG(INFO) << "format found\n";
			break;
		}
	}

	for (int i = 0; i < format.size_count; i++) {
			if ((props.width == format.sizes[i].width) && (props.height == format.sizes[i].height) ) {
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

	unicap_register_callback(handle, UNICAP_EVENT_NEW_FRAME,
			(unicap_callback_t) new_frame_cb, this);
	/*
	 Start the capture process on the device
	 */
	if (!SUCCESS(unicap_start_capture(handle))) {
		LOG(ERROR) << "Failed to start capture on device: "
				<< device.identifier << '\n';

	}
}

void CameraUniCap_Source::finish() {
	LOG(INFO) << "CameraOpenCV_Source::finish()\n";
	/*
	 Stop the device
	 */
	if (!SUCCESS(unicap_stop_capture(handle))) {
		fprintf(stderr, "Failed to stop capture on device: %s\n",
				device.identifier);
	}

	/*
	 Close the device

	 This invalidates the handle
	 */
		if (!SUCCESS(unicap_close(handle))) {
			LOG(ERROR) << "Failed to close the device: " << device.identifier << '\n';
	 }

}

int CameraUniCap_Source::step() {

	return 0;
}

void CameraUniCap_Source::new_frame_cb(unicap_event_t event,
		unicap_handle_t handle, unicap_data_buffer_t *buffer, void *usr_data) {

	Mat frame = Mat(((CameraUniCap_Source*) (usr_data))->format.size.height,
			((CameraUniCap_Source*) (usr_data))->format.size.width,(std::string("GREY") == (char*)&((CameraUniCap_Source*) (usr_data))->format.fourcc) ? CV_8UC1 : CV_8UC3,
			(void *) buffer->data).clone();

	((CameraUniCap_Source*) (usr_data))->out_img.write(frame);
	((CameraUniCap_Source*) (usr_data))->newImage->raise();

}

}

}
