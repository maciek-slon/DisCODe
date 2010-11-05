#include <algorithm>
#include <sstream>
#include <string>

#include "V4L2.hpp"
#include "PrintV4L2.hpp"
#include "PixelFormats.hpp"

#include "Logger.hpp"

namespace Sources {
namespace CameraV4L {

/*!
 * Constructor
 * char * device - device name
 * io_method io_meth - io method to get frame
 */
V4L2::V4L2() {

}

void V4L2::init(const CameraProps & props) {
	dev_name = "/dev/" + props.device;

	fd = -1;
	buffers = NULL;
	n_buffers = 0;

	struct stat st;
	if (-1 == stat(dev_name.c_str(), &st)) {
		LOG(LERROR) << "Cannot identify " << props.device << "\n";
	}

	if (!S_ISCHR(st.st_mode)) {
		LOG(LERROR) << props.device << " is no device\n";
	}

	fd = open(dev_name.c_str(), O_RDWR /* required */| O_NONBLOCK, 0);

	if (-1 == fd) {
		LOG(LERROR) << "Cannot open " << props.device << "\n";
	}
}

const string Int2Char(int i) {
	string s;
	stringstream out;
	out << i;
	s = out.str();
	return s;
}

/*!
 * Destructor
 */
V4L2::~V4L2() {
	releaseFrameGrabber();
	if (-1 == close(fd))
		printf("Cannot close device\n");
	fd = -1;
}

/*!
 * Method prints frame grabber settings.
 */
void V4L2::printSettings() {
	struct v4l2_capability cap;
	if (get(&cap))
		print(cap);
	else
		printf("No capability info\n");

}

/*!
 * Method gets file descriptor.
 */
int V4L2::getFileDescriptor() {
	return fd;
}

/*!
 * (Simple) Method gets standard capabilities.
 */
int V4L2::get(struct v4l2_capability *cap) {
	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, cap))
		return 0;
	else
		return 1;

}

/*!
 * (Simple) Method sets video format.
 */
int V4L2::set(struct v4l2_format fmt) {
	if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
		return 0;
	else
		return 1;
}

/*!
 * (Simple) Method gets video format.
 */
int V4L2::get(struct v4l2_format *fmt) {
	if (-1 == xioctl(fd, VIDIOC_G_FMT, fmt))
		return 0;
	else
		return 1;
}

/*!
 * (Simple) Method gets query variables.
 */
int V4L2::get(struct v4l2_queryctrl *ctrl) {
	if (-1 == xioctl(fd, VIDIOC_QUERYCTRL, ctrl))
		return 0;
	else
		return 1;
}

/*!
 * (Simple) Method sets control variables.
 */
int V4L2::set(struct v4l2_control control) {
	if (-1 == xioctl(fd, VIDIOC_S_CTRL, &control))
		return 0;
	else
		return 1;
}

/*!
 * (Simple) Method gets control variables.
 */
int V4L2::get(struct v4l2_control *control) {
	if (-1 == xioctl(fd, VIDIOC_G_CTRL, control))
		return 0;
	else
		return 1;
}

/*!
 * Method initializes buffers.
 */
void V4L2::initBuffers() {
	struct v4l2_format fmt;
	CLEAR (fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	get(&fmt);
	switch (io) {
	case IO_METHOD_READ:
		init_read(fmt.fmt.pix.sizeimage);
		break;
	case IO_METHOD_MMAP:
		init_mmap();
		break;
	case IO_METHOD_USERPTR:
		init_userp(fmt.fmt.pix.sizeimage);
		break;
	}
}

/*!
 * Method deallocate buffer memory.
 */
void V4L2::freeBuffers() {
	switch (io) {
	case IO_METHOD_READ:
		free(buffers[0].start);
		break;
	case IO_METHOD_MMAP:
		for (int i = 0; i < (int) n_buffers; ++i)
			if (-1 == munmap(buffers[i].start, buffers[i].length))
				printf("munmap\n");
		break;
	case IO_METHOD_USERPTR:
		for (int i = 0; i < (int) n_buffers; ++i)
			free(buffers[i].start);
		break;
	}
	free(buffers);
}

/*!
 * Method starts capture.
 */
void V4L2::startCapture() {
	unsigned int i;
	enum v4l2_buf_type type;
	switch (io) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;
	case IO_METHOD_MMAP:
		for (i = 0; i < n_buffers; ++i) {
			struct v4l2_buffer buf;
			CLEAR (buf);
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			buf.index = i;
			if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
				printf("ERROR: VIDIOC_QBUF\n");
		}
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
			printf("ERROR: VIDIOC_STREAMON\n");
		break;
	case IO_METHOD_USERPTR:
		for (i = 0; i < n_buffers; ++i) {
			struct v4l2_buffer buf;
			CLEAR (buf);
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_USERPTR;
			buf.index = i;
			buf.m.userptr = (unsigned long) buffers[i].start;
			buf.length = buffers[i].length;
			if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
				LOG(LERROR) << "VIDIOC_QBUF\n";
		}
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
			LOG(LERROR) << "VIDIOC_STREAMON\n";
		break;
	}

	cvInitImageHeader(&img, cvSize(getWinProperty(0), getWinProperty(1)),
			IPL_DEPTH_8U, 3, IPL_ORIGIN_TL, 4);
	img.imageData = (char *) cvAlloc(img.imageSize);
	//img.imageData = (char *)cvAlloc(imageBits(palette)*width*height/8);
}

/*!
 * Method stops capture.
 */
void V4L2::stopCapture() {
	enum v4l2_buf_type type;
	switch (io) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;
	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
			LOG(LERROR) << "VIDIOC_STREAMOFF\n";
		break;
	}
	cvFree(&(img.imageData));
}

/*!
 * Method gets one frame.
 */
void *V4L2::getFrame() {
	for (;;) {
		fd_set fds;
		struct timeval tv;
		int r;
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		r = select(fd + 1, &fds, NULL, NULL, &tv);
		if (-1 == r) {
			if (EINTR == errno)
				continue;
			LOG(LWARNING) << "select\n";
		}
		if (0 == r) {
			LOG(LERROR) << "select timeout\n";
		}

		void *p = readFrame();

		return p;
	}
}

/*!
 * Method reads single frame.
 */
void *V4L2::readFrame() {
	struct v4l2_buffer buf;
	unsigned int i;

	switch (io) {
	case IO_METHOD_READ:
		if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
			switch (errno) {
			case EAGAIN:
				return NULL;
			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */
			default:
				printf("read\n");
			}
		}
		return buffers[0].start;
		break;

	case IO_METHOD_MMAP:
		CLEAR (buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
			switch (errno) {
			case EAGAIN:
				return NULL;
			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */
			default:
				LOG(LERROR) << "VIDIOC_DQBUF\n";
			}
		}
		assert(buf.index < n_buffers);
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			LOG(LERROR) << "VIDIOC_QBUF\n";
		return buffers[buf.index].start;
		break;

	case IO_METHOD_USERPTR:
		CLEAR (buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_USERPTR;
		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
			switch (errno) {
			case EAGAIN:
				return NULL;
			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */
			default:
				LOG(LERROR) << "VIDIOC_DQBUF\n";
			}
		}
		for (i = 0; i < n_buffers; ++i)
			if (buf.m.userptr == (unsigned long) buffers[i].start && buf.length
					== buffers[i].length)
				break;
		assert(i < n_buffers);
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			LOG(LERROR) << "VIDIOC_QBUF\n";
		return (void *) (buf.m.userptr);
		break;

	}
	return NULL;
}

/*!
 * Method initializes frame buffer when we choose DMA io_method.
 */
void V4L2::init_read(unsigned int buffer_size) {
	buffers = (buffer *) (calloc(1, sizeof(*buffers)));
	if (!buffers) {
		printf("Out of memory\n");
	}
	buffers[0].length = buffer_size;
	buffers[0].start = malloc(buffer_size);
	if (!buffers[0].start) {
		printf("Out of memory\n");
	}
}

/*!
 * Method initializes frame buffer when we choose MM io_method.
 */
void V4L2::init_mmap(void) {
	struct v4l2_requestbuffers req;
	CLEAR (req);
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			LOG(LERROR) << dev_name << " does not support memory mapping\n";
		} else {
			LOG(LERROR) << "VIDIOC_REQBUFS\n";
		}
	}

	if (req.count < 2) {
		LOG(LERROR) << "Insufficient buffer memory on " << dev_name << "\n";
	}

	buffers = (buffer *) (calloc(req.count, sizeof(*buffers)));
	if (!buffers) {
		LOG(LERROR) << "Out of memory\n";
	}
	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;
		CLEAR (buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
			LOG(LERROR) << "VIDIOC_QUERYBUF\n";
		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap(NULL /* start anywhere */, buf.length,
				PROT_READ | PROT_WRITE /* required */,
				MAP_SHARED /* recommended */, fd, buf.m.offset);

		if (MAP_FAILED == buffers[n_buffers].start)
			LOG(LWARNING) << "mmap\n";
	}
}

/*!
 * Method initializes frame buffer when we choose US io_method.
 */
void V4L2::init_userp(unsigned int buffer_size) {
	struct v4l2_requestbuffers req;
	CLEAR (req);
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;
	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			LOG(LERROR) << dev_name << " does not support user pointer i/o\n";
		} else {
			LOG(LERROR) << "VIDIOC_REQBUFS\n";
		}
	}
	buffers = (buffer *) (calloc(4, sizeof(*buffers)));
	if (!buffers) {
		LOG(LERROR) << "Out of memory\n";
	}

	for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
		buffers[n_buffers].length = buffer_size;
		buffers[n_buffers].start = malloc(buffer_size);

		if (!buffers[n_buffers].start) {
			LOG(LERROR) << "Out of memory\n";
		}
	}
}

/*!
 * Method  sets picture property value like brightness or contrast.

 * 0 - brightness
 * 1 - whiteness
 * 2 - contrast
 * 3 - hue
 */
bool V4L2::setPicProperty(int property, int newValue) {

	struct v4l2_control control;
	CLEAR (control);

	switch (property) {

	case 0:
		control.id = V4L2_CID_BRIGHTNESS;
		break;

	case 1:
		control.id = V4L2_CID_WHITENESS;
		break;

	case 2:
		control.id = V4L2_CID_CONTRAST;
		break;

	case 3:
		control.id = V4L2_CID_HUE;
		break;

	default: {
		printf("property is not supported V4L\n");
		return false;
	}

	}
	control.value = newValue * 65535 / 100;
	if (ioctl(fd, VIDIOC_S_CTRL, &control) < 0) {
		perror("VIDIOC_S_CTRL");
		return false;
	}

	return true;

}

/*!
 * Method  sets window property value like window width or height.
 * int property - property number
 * int newValue - new value
 * 0 - width
 * 1 - height
 * 2 - palette
 */

bool V4L2::setWinProperty(int property, int newValue) {

	struct v4l2_format videoformat;
	CLEAR (videoformat);

	if (property < 0) {
		videoformat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		videoformat.fmt.pix.width = width = 640;
		videoformat.fmt.pix.height = height = 480;
		videoformat.fmt.pix.pixelformat = palette = YUYV_2;
		videoformat.fmt.pix.field = interlace = V4L2_FIELD_INTERLACED;

		if (!set(videoformat)) {
			printf("device does not support settings:\n");
			return false;
		}
		return true;
	} else {
		videoformat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		videoformat.fmt.pix.width = width;
		videoformat.fmt.pix.height = height;
		videoformat.fmt.pix.pixelformat = palette;
		videoformat.fmt.pix.field = interlace;
		//XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();

		switch (property) {
		case 0:
			if (interlace != V4L2_FIELD_INTERLACED and newValue > 320) {
				cout << "couldn't change image size to: " << newValue << endl;
				videoformat.fmt.pix.width = newValue = 320;
			} else
				videoformat.fmt.pix.width = newValue;
			break;
		case 1:
			if (interlace != V4L2_FIELD_INTERLACED and newValue > 240) {
				cout << "couldn't change image size to: " << newValue << endl;
				videoformat.fmt.pix.height = newValue = 240;
			} else
				videoformat.fmt.pix.height = newValue;
			break;
		case 2:
			videoformat.fmt.pix.pixelformat = newValue;
			break;
		case 3:
			if (convInterlace(newValue) != 4) {
				videoformat.fmt.pix.width = 320;
				videoformat.fmt.pix.height = 240;
			}
			videoformat.fmt.pix.field = convInterlace(newValue);
			break;
		default:
			printf("device does not support settings:\n");
		}

		if (!set(videoformat)) {
			printf("device does not support settings:\n");
			return false;
		} else {
			switch (property) {
			case 0:
				width = newValue;
				break;
			case 1:
				height = newValue;
				//xmlds->raiseEvent("CameraV4L_Panel", "image_size", Int2Char(width) + "x" + Int2Char(height));
				break;
			case 2:
				palette = newValue;
				break;
			case 3:
				interlace = convInterlace(newValue);
				/*
				 * wybrana jest opcja inna niz INTERLACED
				 */
				if (convInterlace(newValue) != 4) {
					width = 320;
					height = 240;
					//xmlds->raiseEvent("CameraV4L_Panel", "image_size", "320x240");
				}
				break;
			}
		}

		return true;
	}
}

/*!
 * Method  gets picture property value like brightness or contrast.
 * 0 - brightness
 * 1 - whiteness
 * 2 - contrast
 * 3 - hue
 */
int V4L2::getPicProperty(int property) {

	struct v4l2_control control;
	CLEAR (control);

	int returnValue = 0;

	switch (property) {

	case 0:
		control.id = V4L2_CID_BRIGHTNESS;
		break;

	case 1:
		control.id = V4L2_CID_WHITENESS;
		break;

	case 2:
		control.id = V4L2_CID_CONTRAST;
		break;

	case 3:
		control.id = V4L2_CID_HUE;
		break;

	default:
		printf("property is not supported V4L\n");

	}

	if (ioctl(fd, VIDIOC_G_CTRL, &control) < 0)
		perror("VIDIOC_G_CTRL");

	returnValue = int(100.0 * control.value / 65535);

	return returnValue;

}

/*!
 * Method  gets window property value like window width or height.
 * int property - property number
 * 0 - width
 * 1 - height
 * 2 - palette
 */

int V4L2::getWinProperty(int property) {

	int returnValue = -1;
	struct v4l2_fmtdesc fmtdesc;
	struct v4l2_format videoformat;
	CLEAR (videoformat);
	CLEAR (fmtdesc);
	videoformat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == ioctl(fd, VIDIOC_G_FMT, &videoformat))
		LOG(LERROR) << "VIDIOC_G_FMT\n";

	fmtdesc.index = 0;
	while (0 == ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)) {
		fmtdesc.index++;
	}

	switch (property) {
	case 0:
		returnValue = videoformat.fmt.pix.width;
		break;
	case 1:
		returnValue = videoformat.fmt.pix.height;
		break;
	case 2:
		returnValue = videoformat.fmt.pix.pixelformat;
		break;

	}

	return returnValue;
}

/*!
 * Method  sets video property value like video standard or channel.
 */
bool V4L2::setVideoProperty(int channel, int newValue) {
	/*
	 * if channel == -1, then autosearch
	 */

	struct v4l2_input input;
	CLEAR (input);
	v4l2_std_id std_id;
	int index = channel;

	if (channel < 0) {
		if (-1 == ioctl(fd, VIDIOC_G_INPUT, &index)) {
			perror("VIDIOC_G_INPUT");
			return false;
		}

		input.index = index;
	} else
		input.index = channel;

	if (-1 == ioctl(fd, VIDIOC_ENUMINPUT, &input)) {
		perror("VIDIOC_ENUMINPUT");
		return false;
	}

	switch (newValue) {
	case 0:
		std_id = V4L2_STD_PAL;
		break;
	case 1:
		std_id = V4L2_STD_NTSC;
		break;
	case 2:
		std_id = V4L2_STD_SECAM;
		break;
	default:
		printf("standard is not supported\n");
	}

	if (-1 == ioctl(fd, VIDIOC_S_INPUT, &input)) {
		perror("VIDIOC_S_INPUT");
		return false;
	}

	if (-1 == ioctl(fd, VIDIOC_S_STD, &std_id)) {
		perror("VIDIOC_S_STD");
		return false;
	}

	return true;

}

/*!
 * Method  gets video property value like video standard or channel.
 */
int V4L2::getVideoProperty(int property) {

	struct v4l2_input input;
	CLEAR (input);
	int index;

	if (-1 == ioctl(fd, VIDIOC_G_INPUT, &index))
		perror("VIDIOC_G_INPUT");

	input.index = index;
	if (-1 == ioctl(fd, VIDIOC_ENUMINPUT, &input))
		perror("VIDIOC_ENUMINPUT");

	switch (property) {
	case 0:
		return index;
	case 1:
		return input.std;

	default:
		printf("property is not supported\n");
	}

	return -1;
}

/*!
 * Method gets convInterlace(newValue)one frame from frame grabber.
 */
IplImage * V4L2::getOneFrame() {

	void *frame = getFrame();

	if (!frame) {
		LOG(LERROR) << "Empty frame!\n";
		return NULL;
	}

	unsigned char* tmp = NULL;

	int switchOnConversionToRGB = 1;
	if (switchOnConversionToRGB) {
		switch (palette) {
		case YUYV_2:
			tmp = yuyv_to_rgb24(width, height, (unsigned char*) frame, NULL);
			break;
		case UYVY_2:
			tmp = uyvy_to_rgb24(width, height, (unsigned char*) frame, NULL);
			break;
		case BGR24_2:
			tmp = (unsigned char*) frame;
			break;
		case RGB24_2:
			tmp = (unsigned char*) frame;
			break;
		case YVU420_2:
			tmp = yuv420_to_rgb24(width, height, (unsigned char*) frame, NULL);
			break;
		case YUV422P_2:
			tmp = yuv420p_to_rgb24(width, height, (unsigned char*) frame, NULL);
			break;
		case YUV411P_2:
			tmp = yuv411p_to_rgb24(width, height, (unsigned char*) frame, NULL);
			break;
		}

		memcpy(img.imageData, tmp, 3 * width * height);
		if (tmp != NULL && palette != RGB24_2 && palette != BGR24_2)
			free(tmp);
	} else
		memcpy(img.imageData, frame, (imageBits(palette) * width * height) / 8);

	return &img;
}

/*!
 * Method loads standard settings like brightness, contrast etc (with source_settings).
 */
bool V4L2::loadFrameGrabber(int version, const CameraProps & props) {
	if (version > 0) {
		tryPalettes();

		//if (!findPalette(s_palette))
		//	i_palette = convPalette(*(V4L2_palettes.begin()));

		while (1) {
			if (!setWinProperty(-1, 0))
				break;
			if (!setWinProperty(0, props.width))
				break;
			if (!setWinProperty(1, props.height))
				break;
			if (!setWinProperty(2, props.palette))
				break;
			if (!setWinProperty(3, props.interlace))
				break;
			if (!setPicProperty(0, props.brightness))
				break;
			if (!setPicProperty(2, props.contrast))
				break;
			if (!setPicProperty(3, props.hue))
				break;
			if (!setVideoProperty(-1, 0))
				break;
			if (!setVideoProperty(convChannel(props.channel), 0))
				break;
			if (!setVideoProperty(1, props.standard))
				break;
			break;
		}
	}

	initBuffers();
	startCapture();
	return true;
}

/*!
 * Method gets information about device channels.
 */
vector<string> V4L2::getChannels() {

	vector<string> deviceChannels;
	struct video_channel vidchan;
	CLEAR(vidchan);
	if (-1 == ioctl(fd, VIDIOCGCHAN, &vidchan)) {
		deviceChannels.push_back("none");
	} else {
		struct v4l2_input input;
		for (int channel = 0; channel < 4; channel++) {
			input.index = channel;
			if (-1 == ioctl(fd, VIDIOC_ENUMINPUT, &input))
				break;
			deviceChannels.push_back(string((const char*) input.name));
		}
	}
	return deviceChannels;
}

/*!
 * Method  stops frame grabber and deallocate memory.
 */
bool V4L2::releaseFrameGrabber() {
	stopCapture();
	freeBuffers();
	return true;
}

int V4L2::imageBits(int palette) {
	switch (palette) {
	case V4L2_PIX_FMT_GREY:
	case V4L2_PIX_FMT_RGB332:
		return 8;
	case V4L2_PIX_FMT_YVU410:
	case V4L2_PIX_FMT_YUV410:
		return 9;
	case V4L2_PIX_FMT_YUV411P:
	case V4L2_PIX_FMT_NV12:
	case V4L2_PIX_FMT_NV21:
	case V4L2_PIX_FMT_Y41P:
	case V4L2_PIX_FMT_YVU420:
	case V4L2_PIX_FMT_YUV420:
		return 12;
	case V4L2_PIX_FMT_YUYV:
	case V4L2_PIX_FMT_UYVY:
	case V4L2_PIX_FMT_YUV422P:
	case V4L2_PIX_FMT_YYUV:
	case V4L2_PIX_FMT_RGB555:
	case V4L2_PIX_FMT_RGB565:
	case V4L2_PIX_FMT_RGB555X:
	case V4L2_PIX_FMT_RGB565X:
		return 16;
	case V4L2_PIX_FMT_BGR24:
	case V4L2_PIX_FMT_RGB24:
		return 24;
	case V4L2_PIX_FMT_BGR32:
	case V4L2_PIX_FMT_RGB32:
		return 32;
	}

	/// \todo what is default?
	return 0;
}

void V4L2::tryPalettes() {
	struct v4l2_format videoformat;
	CLEAR (videoformat);
	videoformat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	videoformat.fmt.pix.width = 320;
	videoformat.fmt.pix.height = 240;
	videoformat.fmt.pix.field = V4L2_FIELD_INTERLACED;

	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_GREY;
	if (set(videoformat))
		V4L2_palettes.push_back("GREY_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB332;
	if (set(videoformat))
		V4L2_palettes.push_back("RGB332_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YVU410;
	if (set(videoformat))
		V4L2_palettes.push_back("YVU410_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV410;
	if (set(videoformat))
		V4L2_palettes.push_back("YUV410_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV411P;
	if (set(videoformat))
		V4L2_palettes.push_back("YUV411P_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12;
	if (set(videoformat))
		V4L2_palettes.push_back("NV12_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_NV21;
	if (set(videoformat))
		V4L2_palettes.push_back("NV21_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_Y41P;
	if (set(videoformat))
		V4L2_palettes.push_back("Y41P_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YVU420;
	if (set(videoformat))
		V4L2_palettes.push_back("YVU420_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
	if (set(videoformat))
		V4L2_palettes.push_back("YUV420_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	if (set(videoformat))
		V4L2_palettes.push_back("YUYV_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
	if (set(videoformat))
		V4L2_palettes.push_back("UYVY_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV422P;
	if (set(videoformat))
		V4L2_palettes.push_back("YUV422P_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YYUV;
	if (set(videoformat))
		V4L2_palettes.push_back("YYUV_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB555;
	if (set(videoformat))
		V4L2_palettes.push_back("RGB555_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;
	if (set(videoformat))
		V4L2_palettes.push_back("RGB565_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB555X;
	if (set(videoformat))
		V4L2_palettes.push_back("RGB555X_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565X;
	if (set(videoformat))
		V4L2_palettes.push_back("RGB565X_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
	if (set(videoformat))
		V4L2_palettes.push_back("BGR24_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
	if (set(videoformat))
		V4L2_palettes.push_back("RGB24_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR32;
	if (set(videoformat))
		V4L2_palettes.push_back("BGR32_2");
	videoformat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
	if (set(videoformat))
		V4L2_palettes.push_back("RGB32_2");
}

vector<string> V4L2::getPalettes() {
	return V4L2_palettes;
}

bool V4L2::findPalette(string palette) {
	vector<string>::iterator it;
	it = find(V4L2_palettes.begin(), V4L2_palettes.end(), palette);

	if (it != V4L2_palettes.end())
		return true;
	return false;
}
/*
 * Method return all information about frame grabber
 */
string V4L2::getFrameGrabberOptions() {

	string tmp = "Devices;";
	vector<string>::iterator iter1;
	vector<string> devices = getDevices();
	/*przekazanie urzadzen*/
	for (iter1 = devices.begin(); iter1 != devices.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + "Channels;";

	/*przekazanie kanalow*/
	vector<string> channels = getChannels();
	for (iter1 = channels.begin(); iter1 != channels.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + "Palettes;";

	vector<string> palettes = getPalettes();
	for (iter1 = palettes.begin(); iter1 != palettes.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + "Image_size;";

	vector<string> width = getImageSize();
	for (iter1 = width.begin(); iter1 != width.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	/*		tmp=tmp+"Height;";

	 vector<string> height = getHeight();
	 for(iter1 = height.begin(); iter1!=height.end(); iter1++)
	 tmp=tmp + (*iter1) + ";";*/
	tmp = tmp + "Standard;";

	vector<string> standard = getStandard();
	for (iter1 = standard.begin(); iter1 != standard.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + "Interlace;";

	vector<string> interlace = getInterlace();
	for (iter1 = interlace.begin(); iter1 != interlace.end(); iter1++)
		tmp = tmp + (*iter1) + ";";
	tmp = tmp + "IOMethod;";

	vector<string> iomethod = getIOMethod();
	for (iter1 = iomethod.begin(); iter1 != iomethod.end(); iter1++)
		tmp = tmp + (*iter1) + ";";

	return tmp;
}

vector<string> V4L2::getIOMethod() {
	vector<string> tmp;
	struct v4l2_requestbuffers req;
	CLEAR (req);
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
		if (EINVAL != errno) {
			tmp.push_back("MMAP");
		}
	/*req.memory              = V4L2_MEMORY_USERPTR;
	 if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req))
	 if (EINVAL != errno) {
	 tmp.push_back("USRPTR");
	 }
	 */
	tmp.push_back("READ");

	return tmp;
}

}
}
