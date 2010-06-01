/*!
 * \file VL_Common.hpp
 * \brief
 * \author mstefanc
 * \date May 29, 2010
 */

#ifndef VL_COMMON_HPP_
#define VL_COMMON_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev.h>

typedef enum {
	IO_METHOD_READ,
	IO_METHOD_MMAP,
	IO_METHOD_USERPTR
} io_method;

int xioctl(int fd, int request, void * arg) {
	int r;
	do
		r = ioctl(fd, request, arg);
	while (-1 == r && EINTR == errno);
	return r;
}

int tryLib(const std::string & device, io_method io) {

	int fdes = open(device, O_RDWR /* required */| O_NONBLOCK, 0);
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


#endif /* VL_COMMON_HPP_ */
