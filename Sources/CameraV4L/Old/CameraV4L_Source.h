/*!
 * \file CameraV4L_Source.h
 * \brief
 *
 * \author  tkornuta
 * \date Nov 12, 2009
 */

#ifndef CAMERAV4L_SOURCE_H_
#define CAMERAV4L_SOURCE_H_

#include "ISource.h"
#include "V4L.h"
#include "Camera_Source.h"

namespace Core {

namespace Sources {

class CameraV4L_Source: public Core::Sources::Camera_Source
{

	VL * cam;

	/*!
	 * Single event handler. Returns true if handled correctly.
	 */
	bool handleSingleEvent(string attribute_, string value_);

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	CameraV4L_Source();

	~CameraV4L_Source();

	/*!
	 * Method loads configuration from file.
	 */
	bool loadConfiguration();

	bool restartConfiguration_io();

	bool restartConfiguration_dev();
	/*!
	 * Method saves configuration to file.
	 */
	bool saveConfiguration();

	/*!
	 * Retrieves image from movie.
	 */
	void grabImage();

	/*!
	 * Returns retrieved image.
	 */
	IplImage* returnImage();

	/*!
	 * Connects source to given device.
	 */
	void connect();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	void disconnect();

};

}

}

#endif /* CAMERAV4L_SOURCE_H_ */
