/*!
 * \file CameraV4L_Source.cc
 * \brief
 *
 * \author  tkornuta
 * \date Nov 12, 2009
 */
#include "V4L2.h"
#include "V4L.h"
#include <stdio.h>
#include <string>
#include "CameraV4L_Source.h"
#include "FraDIAException.h"

using namespace std;
using namespace Common;

namespace Core {
namespace Sources {

CameraV4L_Source::CameraV4L_Source() :
	Core::Sources::Camera_Source("CameraV4L_Source", "/settings/sources/CameraV4L")
{
}

CameraV4L_Source::~CameraV4L_Source()
{
	disconnect();
}

void CameraV4L_Source::connect()
{
	int whichCam = VL::tryLib();
	switch (whichCam)
	{
		case 0:
			cam = NULL;
			break;
		case 1:
			cam = new V4L();
			break;
		case 2:
			cam = new V4L2();
			break;
		default:
			cam = NULL;
	}
	if (cam != NULL) {
		cam->loadFrameGrabber(1);
	}
}

void CameraV4L_Source::disconnect()
{
	if (cam != NULL)
		delete cam;
	// Image memory block is freed by the V4L destructor.
	image = NULL;
}

bool CameraV4L_Source::loadConfiguration()
{
	// Return true if configuration was loaded properly.
	// get all information about frame grabber

	if (cam != NULL) {
		string startSettings = cam->getFrameGrabberOptions();
		xmlds->raiseEvent("CameraV4L_Panel", "startSettings", startSettings);
	}

	return true;
}

bool CameraV4L_Source::restartConfiguration_io()
{
	if (cam != NULL) {
		string startSettings = cam->getFrameGrabberOptions();
		xmlds->raiseEvent("CameraV4L_Panel", "restartSettings_io", startSettings);
	}

	return true;
}

bool CameraV4L_Source::restartConfiguration_dev()
{
	if (cam != NULL) {
		string startSettings = cam->getFrameGrabberOptions();
		xmlds->raiseEvent("CameraV4L_Panel", "restartSettings_dev", startSettings);
	}

	return true;
}

bool CameraV4L_Source::saveConfiguration()
{
	// Return true if configuration was saved properly.
	return true;
}

bool CameraV4L_Source::handleSingleEvent(string attribute_, string value_)
{

	if (cam != NULL)
		if (attribute_.compare("device") == 0) {
			if (cam != NULL)
				delete cam;

			int whichCam = VL::tryLib();
			switch (whichCam)
			{
				case 1:
					cam = new V4L("device", value_);
					restartConfiguration_dev();
					break;
				case 2:
					cam = new V4L2("device", value_);
					restartConfiguration_dev();
					break;
				default:
					cam = NULL;
			}

			if (cam != NULL) {
				cam->loadFrameGrabber(1);
			} else
				image = NULL;
		} else if (attribute_.compare("io_method") == 0) {
			if (cam != NULL)
				delete cam;

			int whichCam = VL::tryLib();
			switch (whichCam)
			{
				case 1:
					cam = new V4L("io_method", value_);
					restartConfiguration_io();
					break;
				case 2:
					cam = new V4L2("io_method", value_);
					restartConfiguration_io();
					break;
				default:
					cam = NULL;
			}

			if (cam != NULL) {
				cam->loadFrameGrabber(1);
			} else
				image = NULL;
		} else if (attribute_.compare("image_size") == 0) {
			/*
			 * podzia string z postaci aXb na a i b
			 */
			size_t pos;
			string width;
			string height;
			pos = value_.find("x");
			width = value_.substr(0, pos);
			height = value_.substr(pos + 1);

			cam->releaseFrameGrabber();
			cam->setWinProperty(0, atoi(width.c_str()));
			cam->setWinProperty(1, atoi(height.c_str()));
			cam->loadFrameGrabber(0);
		}

		else if (attribute_.compare("video_input") == 0)
			cam->setVideoProperty(cam->convChannel(value_.c_str()), 0);
		else if (attribute_.compare("video_standard") == 0)
			cam->setVideoProperty(-1, cam->convStandard(value_.c_str()));
		else if (attribute_.compare("palette") == 0) {
			cam->releaseFrameGrabber();
			cam->setWinProperty(2, cam->convPalette(value_.c_str()));
			cam->loadFrameGrabber(0);
		} else if (attribute_.compare("interlace") == 0) {
			cam->releaseFrameGrabber();
			cam->setWinProperty(3, cam->convInterlace2String(value_));
			cam->loadFrameGrabber(0);
		} else if (attribute_.compare("brightness") == 0)
			cam->setPicProperty(0, atoi(value_.c_str()));
		else if (attribute_.compare("whiteness") == 0)
			cam->setPicProperty(1, atoi(value_.c_str()));
		else if (attribute_.compare("contrast") == 0)
			cam->setPicProperty(2, atoi(value_.c_str()));
		else if (attribute_.compare("hue") == 0)
			cam->setPicProperty(3, atoi(value_.c_str()));

	return true;
}

void CameraV4L_Source::grabImage()
{
	if (cam != NULL) {
		image = cam->getOneFrame();
	} else
		image = NULL;
	// Check frame.
	if (image == NULL) {
		throw FraDIAException("FraDIA: Retrieved empty frame.\n");
	}
	// Correct retrieved frame - remove distortion.
	doCorrection();
}

IplImage* CameraV4L_Source::returnImage()
{
	// Return image.
	return getCorrectedImage();
}

}//namespace Sources
}//namespace Core
