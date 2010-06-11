/*!
 * \file VL.h
 * \author szymek
 * \date 05.03.2009
 */

#ifndef VL_HPP_
#define VL_HPP_

#include <string>

#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <stdio.h>
#include "cv.h"
#include "cvaux.h"

#include "V4L_define.hpp"
#include "VL_Common.hpp"
#include "CameraProps.hpp"

#include <iostream>
#include <vector>

using namespace std;

namespace Sources {
namespace CameraV4L {

/*!
 * \class VL
 * \author szymek
 */
class VL {

public:

	VL() {
	}

	VL(string param, string value) {
	}


	virtual ~VL() {
	}

	/*!
	 * Initialize object with given properties
	 * @param props
	 */
	virtual void init(const CameraProps & props)=0;

	/*!
	 * Method loads standard settings like brightness, contrast etc (with source_settings).
	 */
	virtual bool loadFrameGrabber(int i, const CameraProps & props)=0;

	/*!
	 * Method gets one frame from frame grabber.
	 */
	virtual IplImage * getOneFrame()=0;

	/*!
	 * Method  stops frame grabber and deallocate memory.
	 */
	virtual bool releaseFrameGrabber()=0;

	/*!
	 * Method  gets video property value like video standard or channel.
	 */
	virtual int getVideoProperty(int property)=0;

	/*!
	 * Method  gets window property value like window width or height.
	 */
	virtual int getWinProperty(int property)=0;

	/*!
	 * Method  gets picture property value like brightness or contrast.
	 */
	virtual int getPicProperty(int property)=0;

	/*!
	 * Method  sets video property value like video standard or channel.
	 */
	virtual bool setVideoProperty(int channel, int norm)=0;

	/*!
	 * Method  sets window property value like window width or height.
	 */
	virtual bool setWinProperty(int property, int value)=0;

	/*!
	 * Method  sets picture property value like brightness or contrast.
	 */
	virtual bool setPicProperty(int property, int value)=0;

	/*!
	 * Method gets number of frame grabber channels.
	 */
	virtual vector<string> getChannels()=0;

	/*!
	 * Method returns information about frame grabber
	 */
	virtual string getFrameGrabberOptions()=0;

	/*!
	 * Method gets devices name
	 */
	virtual vector<string> getDevices();

	/*!
	 * Method gets palettes name
	 */
	virtual vector<string> getPalettes()=0;

	/*!
	 * Method gets width
	 */
	virtual vector<string> getWidth();

	/*!
	 * Method gets height
	 */
	virtual vector<string> getHeight();

	virtual vector<string> getImageSize();

	/*!
	 * Method gets standard
	 */
	virtual vector<string> getStandard();

	virtual vector<string> getInterlace();

	virtual vector<string> getIOMethod()=0;

	/*
	 * Method converts string to int
	 */
	virtual int convPalette(string palette);

	/*
	 * Method converts string to int
	 */
	virtual int convChannel(string palette);

	/*
	 * Method converts string to int
	 */
	virtual int convStandard(string palette);

	virtual v4l2_field convInterlace(int i);

	virtual int convInterlace2String(string i);

	static io_method convIOMethod(string method);
};

}
}

#endif /* _VL_HPP_ */

