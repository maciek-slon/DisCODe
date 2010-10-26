#include "VL_Common.hpp"

namespace Sources {
namespace CameraV4L {

int xioctl(int fd, int request, void * arg) {
	int r;
	do
		r = ioctl(fd, request, arg);
	while (-1 == r && EINTR == errno);
	return r;
}

int tryLib(const std::string & device, io_method io) {

	int fdes = open(device.c_str(), O_RDWR /* required */| O_NONBLOCK, 0);
	if (fdes >= 0) {
		struct v4l2_capability cap;
		CLEAR(cap);
		if (io != IO_METHOD_READ) {
			if (xioctl(fdes, VIDIOC_QUERYCAP, &cap) == -1) {
				close(fdes);
				return 1;
			} else {
				struct video_capability caps;
				CLEAR(caps);
				caps.type = cap.capabilities;
				if (xioctl(fdes, VIDIOC_G_INPUT, &(caps.channels)) != -1) {
					close(fdes);
					return 2;
				}
			}
		}
	} else {
		return 0;
	}
}

io_method convIOMethod(const std::string & method) {
	if (method.compare("MMAP") == 0)
		return IO_METHOD_MMAP;
	if (method.compare("READ") == 0)
		return IO_METHOD_READ;
	if (method.compare("USRPTR") == 0)
		return IO_METHOD_USERPTR;

	/// \todo default return?
	return IO_METHOD_MMAP;
}

std::string convIOMethod(io_method meth) {
	switch(meth) {
		case IO_METHOD_MMAP:
			return "MMAP";
		case IO_METHOD_READ:
			return "READ";
		case IO_METHOD_USERPTR:
			return "USERPTR";
	}

	return "MMAP";
}

int convStandard(const std::string & standard) {
	if (standard == "PAL")
		return STD_PAL;
	else if (standard == "NTSC")
		return STD_NTSC;
	else if (standard == "SECAM")
		return STD_SECAM;
	else
		return STD_PAL;
}

std::string convStandard(int standard) {
	switch (standard) {
		case STD_PAL:
			return "PAL";
		case STD_NTSC:
			return "NTSC";
		case STD_SECAM:
			return "SECAM";
	}

	return "PAL";
}

}
}
