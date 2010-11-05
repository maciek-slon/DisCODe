#include "V4L.hpp"
#include "PixelFormats.hpp"

#include "Logger.hpp"

#include <iostream>

using namespace std;

namespace Sources {
namespace CameraV4L {

/*!
 * Constructor.
 */
V4L::V4L() {

}

void V4L::init(const CameraProps & props) {
	dev_name = "/dev/" + props.device;

	if (openDevice()) {
		getDeviceCapabilities();
		getVideoProperty(0);
		getBufferSize();

	} else
		LOG(LERROR) << "function: V4L\n";
}

/*!
 * Method opens device.
 */
bool V4L::openDevice() {
	video_dev = open(dev_name.c_str(), O_RDONLY);
	if (video_dev < 0) {
		LOG(LERROR) << "function: openDevice\n";
		return false;
	}
	return true;
}

/*!
 * Destructor.
 */
V4L::~V4L() {
	closeDevice();
}

/*!
 * Method closes device.
 */
void V4L::closeDevice() {
	if (video_dev != -1)
		close(video_dev);
}

/*!
 * Method gets number of frame grabber channels.
 */
vector<string> V4L::getChannels() {
	vector<string> deviceChannels;

	struct video_capability vidcap;
	if (ioctl(video_dev, VIDIOCGCAP, &vidcap) != -1) {
		for (int i = 0; i < vidcap.channels; i++) {
			struct video_channel vidchan;
			if (-1 == ioctl(video_dev, VIDIOCGCHAN, &vidchan))
				continue;
			deviceChannels.push_back(string((const char*) vidchan.name));
		}
	}
	return deviceChannels;
}

/*!
 * Method gets actual device capabilities.
 */
bool V4L::getDeviceCapabilities() {

	if (ioctl(video_dev, VIDIOCGCAP, &capabilities) == -1) {
		LOG(LERROR) << "function: getDeviceCapabilities\n";
		close(video_dev);
		return false;
	}
	return true;
}

/*!
 * Method  gets video property value like video standard or channel.
 * int property - property number
 *
 */
int V4L::getVideoProperty(int property) {
	int i;
	for (i = 0; i < capabilities.channels; i++) {
		vidchan.channel = i;
		if (ioctl(video_dev, VIDIOCGCHAN, &vidchan) == -1) {
			LOG(LERROR) << "function: getChannelCapabilities\n";
			return 0;
		}
	}
	return 1;
}

/*!
 * Method  sets video property value like video standard or channel.
 * int number - channel number
 * int norm - norm number: 0 - PAL, 1 - NTSC, 2 - SECAM
 */
bool V4L::setVideoProperty(int number, int norm) {
	vidchan.channel = number;
	vidchan.norm = norm;

	if (ioctl(video_dev, VIDIOCSCHAN, &vidchan) == -1) {
		LOG(LERROR) << "function: setChannelCapabilities\n";
		return false;
	} else
		return true;

}

/*!
 * Method  gets window property value like window width or height.
 * int property - property number
 * 0 - width
 * 1 - height
 * 2 - palette
 */
int V4L::getWinProperty(int property) {

	if (property < 2) {
		if (ioctl(video_dev, VIDIOCGWIN, &win) == -1) {
			LOG(LERROR) << "function: getWinProperties\n";
			return 0;
		} else {
			if (property == 0)
				return win.width;
			else
				return win.height;
		}
	} else
		return pic.palette;
}

/*!
 * Method  sets window property value like window width or height.
 * int property - property number
 * 0 - width
 * 1 - height
 * 2 - palette
 */
bool V4L::setWinProperty(int property, int value) {
	if (property < 0) {
		win.x = win.y = win.chromakey = 0;
		win.width = capabilities.maxwidth;
		win.height = capabilities.maxheight;
		win.flags = 0;

	} else {

		switch (property) {
		case 0:
			win.width = value;
			break;
		case 1:
			win.height = value;
			break;
		case 2:
			pic.palette = value;
			break;
		default:
			printf("property is not supported V4L\n");
			return false;
		}
	}
	if (property != 2) {
		if (ioctl(video_dev, VIDIOCSWIN, &win) == -1) {
			LOG(LERROR) << "function: setWinProperties\n";
			return false;
		} else
			return true;
	} else {
		if (ioctl(video_dev, VIDIOCSPICT, &pic) == -1) {
			LOG(LERROR) << "function: setWinProperties\n";

			return false;
		} else
			return true;
	}
}

/*!
 * Method  gets picture property value like brightness or contrast.
 * 0-brightness
 * 3-hue
 * 4-colour
 * 2-contrast
 * 1-whiteness
 */
int V4L::getPicProperty(int property) {

	int value = 0;
	switch (property) {

	case 0:
		value = (pic.brightness / 65535) * 100;
		break;
	case 3:
		value = (pic.hue / 65535) * 100;
		break;
	case 4:
		value = (pic.colour / 65535) * 100;
		break;
	case 2:
		value = (pic.contrast / 65535) * 100;
		break;
	case 1:
		value = (pic.whiteness / 65535) * 100;
		break;
	default:
		printf("property is not supported V4L\n");
	}
	return value;
}

/*!
 * Method  sets picture property value like brightness or contrast.
 * 0-brightness
 * 3-hue
 * 4-colour
 * 2-contrast
 * 1-whiteness
 */
bool V4L::setPicProperty(int property, int value) {

	if (property < 0) {

		pic.depth = COLOUR;
		pic.palette = RGB24;
		pic.brightness = pic.hue = pic.colour = pic.contrast = pic.whiteness
				= 32767;
	} else {

		switch (property) {
		case 0:
			pic.brightness = value * 65535 / 100;
			break;
		case 3:
			pic.hue = value * 65535 / 100;
			break;
		case 4:
			pic.colour = value * 65535 / 100;
			break;
		case 2:
			pic.contrast = value * 65535 / 100;
			break;
		case 1:
			pic.whiteness = value * 65535 / 100;
			break;
		default:
			printf("property is not supported V4L\n");
		}
	}

	if (ioctl(video_dev, VIDIOCSPICT, &pic) == -1) {
		LOG(LERROR) << "function: setPicProperties\n";
		return false;
	} else
		return true;
}

/*!
 * Method gets buffer size for single frame.
 */
bool V4L::getBufferSize() {

	/* Retrieve sizes and offsets */
	if (ioctl(video_dev, VIDIOCGMBUF, &m_buf) == -1) {
		LOG(LERROR) << "function: getBufferSize\n";
		return false;
	}

	return true;
}

/*!
 * Method sets memory for buffer.
 */
bool V4L::setMemBuf() {
	map = (char*) mmap(0, m_buf.size, PROT_READ, MAP_SHARED, video_dev, 0);
	if ((unsigned char *) -1 == (unsigned char *) video_dev) {
		LOG(LERROR) << "function: setMemBuf\n";
		return false;
	}
	return true;
}

/*!
 * Method sets memory for memory mapping.
 */
bool V4L::setMemMap() {

	v_map = (struct video_mmap *) (malloc(m_buf.frames
			* sizeof(struct video_mmap)));

	cvInitImageHeader(&frame, cvSize(win.width, win.height), IPL_DEPTH_8U, 3,
			IPL_ORIGIN_TL, 4);

	frame.imageData = (char *) cvAlloc(frame.imageSize);

	FirstCapture = 1;

	/// \todo: always true?
	return true;
}

/*!
 * Method gets one frame (raw data).
 */
bool V4L::grabFrame() {
	if (FirstCapture) {
		for (bufferIndex = 0; bufferIndex < (m_buf.frames) - 1; ++bufferIndex) {
			(*(v_map + bufferIndex)).frame = bufferIndex;
			(*(v_map + bufferIndex)).width = win.width;
			(*(v_map + bufferIndex)).height = win.height;
			(*(v_map + bufferIndex)).format = pic.palette;

			if (ioctl(video_dev, VIDIOCMCAPTURE, (v_map + bufferIndex)) == -1) {
				printf(" bug ERROR: function: grabFrame\n");
				return false;
			}
		}
		FirstCapture = 0;
	}

	(*(v_map + bufferIndex)).frame = bufferIndex;
	(*(v_map + bufferIndex)).width = win.width;
	(*(v_map + bufferIndex)).height = win.height;
	(*(v_map + bufferIndex)).format = pic.palette;

	if (ioctl(video_dev, VIDIOCMCAPTURE, (v_map + bufferIndex)) == -1) {
		return true;
	}

	++bufferIndex;
	if (bufferIndex == m_buf.frames) {
		bufferIndex = 0;
	}

	return true;
}

/*!
 * Method returns one frame (IplImage).
 */
bool V4L::retFrame() {
	if (ioctl(video_dev, VIDIOCSYNC, &((*(v_map + bufferIndex)).frame)) == -1) {
		LOG(LERROR) << "function: retFrame\n";
		return false;
	}

	if ((frame.width != (*(v_map + bufferIndex)).width) || (frame.height
			!= (*(v_map + bufferIndex)).height)) {
		cvFree(&(frame.imageData));
		cvInitImageHeader(&frame, cvSize(win.width, win.height), IPL_DEPTH_8U,
				3, IPL_ORIGIN_TL, 4);
		frame.imageData = (char *) cvAlloc(frame.imageSize);

	}

	memcpy((char *) (frame.imageData), (char *) (map
			+ m_buf.offsets[bufferIndex]), frame.imageSize);
	//memcpy((char *)(original_frame.imageData),(char *)(map + m_buf.offsets[bufferIndex]),original_frame.imageSize);
	/*
	 switch(pic.palette)
	 {
	 case RGB24:
	 memcpy((char *)(frame.imageData),(char *)(map + m_buf.offsets[bufferIndex]),frame.imageSize);
	 break;
	 case YUV422P:
	 yuv420p_to_rgb24(win.width,
	 win.height,
	 (unsigned char*)(map + m_buf.offsets[bufferIndex]),
	 (unsigned char*)frame.imageData);
	 break;
	 case YUV420:
	 yuv420_to_rgb24(win.width,
	 win.height,
	 (unsigned char*)(map + m_buf.offsets[bufferIndex]),
	 (unsigned char*)frame.imageData);
	 break;
	 case YUV411P:
	 yuv411p_to_rgb24(win.width,
	 win.height,
	 (unsigned char*)(map + m_buf.offsets[bufferIndex]),
	 (unsigned char*)frame.imageData);
	 break;
	 default:
	 printf("format is not supported V4L\n");
	 return false;
	 }*/
	return true;
}
/*!
 * Method gets one frame from frame grabber.
 */
IplImage * V4L::getOneFrame() {
	if (grabFrame() && retFrame()) {
		return &frame;
	} else
		return NULL;
}

/*!
 * Method loads standard settings like brightness, contrast etc (with source_settings).
 */
bool V4L::loadFrameGrabber(int version, const CameraProps & props) {

	if (version > 0) {

		/// \todo: use CameraProps

		if (version == 1) {
			setWinProperty(-1, 0);
			setWinProperty(0, props.width);
			setWinProperty(1, props.height);
			setWinProperty(2, props.palette);
		} else {
			if (!setWinProperty(-1, 0))
				return false;
			if (!setVideoProperty(-1, 0))
				return false;
			if (!setVideoProperty(1, 0))
				return false;
			if (!setVideoProperty(1, props.standard))
				return false;
			if (!setWinProperty(0, props.width))
				return false;
			if (!setWinProperty(1, props.height))
				return false;
			if (!setWinProperty(2, props.palette))
				return false;
			if (!setPicProperty(0, props.brightness))
				return false;
			if (!setPicProperty(1, props.whiteness))
				return false;
			if (!setPicProperty(2, props.contrast))
				return false;
			if (!setPicProperty(3, props.hue))
				return false;
		}
	}

	if (!setMemBuf())
		return false;
	if (!setMemMap())
		return false;

	return true;
}

/*!
 * Method  stops frame grabber and deallocate memory.
 */
bool V4L::releaseFrameGrabber() {
	if (map)
		munmap(map, m_buf.size);
	if (frame.imageData)
		cvFree(&(frame.imageData));
	free(v_map);
	return true;
}

vector<string> V4L::getPalettes() {
	vector<string> tmp;
	tmp.push_back("RGB");
	tmp.push_back("BGR");
	tmp.push_back("YUYV");
	tmp.push_back("UYVY");
	return tmp;
}

string V4L::getFrameGrabberOptions() {

	string tmp = "Devices;";
	vector<string>::iterator iter1;
	vector<string> devices = getDevices();
	/*przekazanie urzadzen*/
	for (iter1 = devices.begin(); iter1 != devices.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + ";Channels;";

	/*przekazanie kanalow*/
	vector<string> channels = getChannels();
	for (iter1 = channels.begin(); iter1 != channels.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + ";Palettes;";

	vector<string> palettes = getPalettes();
	for (iter1 = palettes.begin(); iter1 != palettes.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + ";Width;";

	vector<string> width = getWidth();
	for (iter1 = width.begin(); iter1 != width.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + ";Height;";

	vector<string> height = getHeight();
	for (iter1 = height.begin(); iter1 != height.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + ";Standard;";

	vector<string> standard = getStandard();
	for (iter1 = standard.begin(); iter1 != standard.end(); iter1++)
		tmp = tmp + (*iter1) + ";";

	cout << "poszlo" << endl;
	return tmp;
}

vector<string> V4L::getIOMethod() {
	vector<string> tmp;
	tmp.push_back("none");
	return tmp;
}

}
}
