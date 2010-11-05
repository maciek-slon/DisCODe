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
#include <string>
#include <cstdlib>
#include <cstring>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

#define STD_PAL 0
#define STD_NTSC 1
#define STD_SECAM 2

namespace Sources {
namespace CameraV4L {


typedef enum {
	IO_METHOD_READ,
	IO_METHOD_MMAP,
	IO_METHOD_USERPTR
} io_method;


/*!
 *
 * @param fd
 * @param request
 * @param arg
 * @return
 */
int xioctl(int fd, int request, void * arg);

/*!
 *
 * @param device name of device to probe
 * @param io io method to check
 * @return
 */
int tryLib(const std::string & device, io_method io);

/*!
 * Convert string to io_method
 * @param meth string
 * @return io_method
 */
io_method convIOMethod(const std::string & meth);

std::string convIOMethod(io_method meth);

/*!
 * Convert string do video standard
 * @param std
 * @return
 */
int convStandard(const std::string & standard);

std::string convStandard(int standard);

}
}

#endif /* VL_COMMON_HPP_ */
