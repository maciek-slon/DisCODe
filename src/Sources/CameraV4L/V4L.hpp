/*!
 * \file V4L.h
 * \author szymek
 * \date 05.03.2009
 */

#ifndef _V4L_H_
#define _V4L_H_

#include <unistd.h>

#include <sys/time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/types.h>

//opencv
#include "cv.h"
#include "cvaux.h"

#include <linux/videodev.h>
#include "VL.hpp"
#include "CameraProps.hpp"

namespace Sources {
namespace CameraV4L {

/*!
 * \class V4L
 * \author szymek
 */
class V4L: public VL {
private:
	/*!
	 * Device name.
	 */
	string dev_name;
	/*!
	 * Device descriptor.
	 */
	int video_dev;
	/*!
	 * Buffer index.
	 */
	int bufferIndex;
	/*!
	 * First capture flag.
	 */
	int FirstCapture;
	/*!
	 * Struct video_capability where we can find information about standard capabilities.
	 */
	struct video_capability capabilities;
	/*!
	 * Struct video_channel where we can find information about channels.
	 */
	struct video_channel vidchan;
	/*!
	 * Struct video_window where we can find information about window.
	 */
	struct video_window win;
	/*!
	 * Struct video_picture where we can find information about picture.
	 */
	struct video_picture pic;
	/*!
	 * Struct video_mbuf where we can fi
	V4L(string param, string value);nd information about buffer for single frame from frame grabber.
	 */
	struct video_mbuf m_buf;
	/*!
	 * Pointer to frame with data.
	 */
	char * map;
	/*!
	 * Pointer to struct video_mmap.
	 */
	struct video_mmap * v_map;
	/*!
	 * Single frame.
	 */
	IplImage frame;
	/*!
	 * (Original) Single frame.
	 */
	IplImage original_frame;

public:

	void init(const CameraProps & props);

	/*!
	 * Method opens device.
	 */
	bool openDevice();
	/*!
	 * Method closes device.
	 */
	void closeDevice();

	/*!
	 * Method gets and shows actual device capabilities.
	 */
	bool getDeviceCapabilities();
	/*!
	 * Method gets buffer size for single frame.
	 */
	bool getBufferSize();

	/*!
	 * Method sets memory for buffer.
	 */
	bool setMemBuf();
	/*!
	 * Method sets memory for memory mapping.
	 */
	bool setMemMap();
	/*!
	 * Method gets one frame (raw data).
	 */
	bool grabFrame();
	/*!
	 * Method returns one frame (IplImage).
	 */
	bool retFrame();

	/*!
	 * Constructor.
	 */
	V4L();

	/*!
	 * Destructor.
	 */
	~V4L();
	/*!
	 * Method loads standard settings like brightness, contrast etc (without source_settings).
	 */
	bool loadFrameGrabber(int i, const CameraProps & props);
	/*!
	 * Method gets one frame from frame grabber.
	 */
	IplImage * getOneFrame();
	/*!
	 * Method  stops frame grabber and deallocate memory.
	 */
	bool releaseFrameGrabber();

	/*!
	 * Method  gets video property value like video standard or channel.
	 */
	int getVideoProperty(int property);
	/*!
	 * Method  gets window property value like window width or height.
	 */
	int getWinProperty(int property);
	/*!
	 * Method  gets picture property value like brightness or contrast.
	 */
	int getPicProperty(int property);

	/*!
	 * Method  sets video property value like video standard or channel.
	 */
	bool setVideoProperty(int channel, int norm);
	/*!
	 * Method  sets window property value like window width or height.
	 */
	bool setWinProperty(int property, int value);
	/*!
	 * Method  sets picture property value like brightness or contrast.
	 */
	bool setPicProperty(int property, int value);
	/*!
	 * Method gets number of frame grabber channels.
	 */
	vector<string> getChannels();

	vector<string> getPalettes();

	vector<string> getIOMethod();
	/*
	 * Method return all information about frame grabber
	 */
	string getFrameGrabberOptions();
};

}
}

#endif
