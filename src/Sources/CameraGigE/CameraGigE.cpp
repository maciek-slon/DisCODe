/*!
 * \file Sequence.cpp
 * \brief Class responsible for retrieving images from image sequences - methods definition.
 */

#include "CameraGigE.hpp"

#include "Utils.hpp"

namespace Sources {
namespace CameraGigE {

CameraGigE::CameraGigE(const std::string & name) : Base::Component(name) {
	LOG(LTRACE) << "Hello CameraGigE from dl\n";

	if(PvInitialize() == ePvErrResources)
	{
		LOG(LERROR) << "Unable to initialize GigE !!! \n";
		//return false;
	}
}

CameraGigE::~CameraGigE() {
	LOG(LTRACE) << "Goodbye CameraGigE from dl\n";

	PvUnInitialize();
}

bool CameraGigE::onInit() {
	LOG(LTRACE) << "CameraGigE::initialize\n";

	h_onTrigger.setup(this, &CameraGigE::onTrigger);
	registerHandler("onTrigger", &h_onTrigger);

	newImage = registerEvent("newImage");
	endOfSequence = registerEvent("endOfSequence");

	registerStream("out_img", &out_img);

	if(!props.address.empty())
	{
		unsigned long ip = inet_addr(props.address.c_str());

		if(PvCameraOpenByAddr(ip,ePvAccessMaster,&cHandle) != ePvErrSuccess)
		{
			LOG(LERROR) << "Unable to open camera on adress " << props.address << " \n";
			return false;
		}

	} else if(props.uid != 0)
	{
		if(PvCameraOpen(props.uid, ePvAccessMaster, &cHandle) != ePvErrSuccess)
		{
			LOG(LERROR) << "Unable to open camera with uid " << props.uid << " \n";
			return false;
		}
	} else
	{
		return false;
	}

	PvAttrEnumSet(cHandle,"FrameStartTriggerMode","Freerun");

	unsigned long frameSize = 0;

	if(PvAttrUint32Get(cHandle,"TotalBytesPerFrame",&frameSize) != ePvErrSuccess)
	{
		return false;
	}

	frame.ImageBuffer = new char[frameSize];
	frame.ImageBufferSize = frameSize;

	return true;
}

bool CameraGigE::onFinish() {
	LOG(LTRACE) << "CameraGigE::finish\n";

	return true;
}

bool CameraGigE::onStep() {
	LOG(LTRACE) << "CameraGigE::onStep";

	tPvErr Err = PvCaptureQueueFrame(cHandle,&frame,NULL);
    if(!Err)
    {
        Err = PvCaptureWaitForFrameDone(cHandle,&frame,PVINFINITE);
        if(!Err)
        {
            if(frame.Status == ePvErrSuccess)
            {
            	img = cv::Mat(frame.Height,
						  frame.Width,
						  CV_8UC3, frame.ImageBuffer);

            	out_img.write(img);
            	newImage->raise();
            }
        }
    }

	return true;
}

bool CameraGigE::onStart() {
    // set the camera is acquisition mode
    if(!PvCaptureStart(cHandle))
    {
        // start the acquisition and make sure the trigger mode is "freerun"
        if(PvCommandRun(cHandle,"AcquisitionStart"))
        {
            // if that fail, we reset the camera to non capture mode
            PvCaptureEnd(cHandle) ;
            return false;
        }
        else
            return true;
    } else
    	return false;
}

bool CameraGigE::onStop() {
    PvCommandRun(cHandle,"AcquisitionStop");
    PvCaptureEnd(cHandle);
	return true;
}

void CameraGigE::onTrigger() {

}

}//: namespace Sequence
}//: namespace Sources
