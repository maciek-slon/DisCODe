/*!
 * \file Sequence.cpp
 * \brief Class responsible for retrieving images from image sequences - methods definition.
 */

#include "CameraGigE.hpp"

#include "Utils.hpp"

namespace Sources {
namespace CameraGigE {

CameraGigE::CameraGigE(const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello CameraGigE from dl\n";

	if (PvInitialize() == ePvErrResources) {
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

	if (!props.address.empty()) {
		unsigned long ip = inet_addr(props.address.c_str());

		if (PvCameraOpenByAddr(ip, ePvAccessMaster, &cHandle) != ePvErrSuccess) {
			LOG(LERROR) << "Unable to open camera on adress "
					<< props.address << " \n";
			return false;
		}

	} else if (props.uid != 0) {
		if (PvCameraOpen(props.uid, ePvAccessMaster, &cHandle) != ePvErrSuccess) {
			LOG(LERROR) << "Unable to open camera with uid " << props.uid
					<< " \n";
			return false;
		}
	} else {
		return false;
	}

	// Set parameters
	tPvErr err;
	///	 Exposure
	if (!props.exposureMode.empty()) {
		if ((err = PvAttrEnumSet(cHandle, "ExposureMode",
				props.exposureMode.c_str())) == ePvErrSuccess) {
			if (props.exposureMode == "Manual") {
				if ((err = PvAttrUint32Set(cHandle, "ExposureValue",
						props.exposureValue / 1000000.0)) != ePvErrSuccess) {
					if (err == ePvErrOutOfRange) {
						tPvUint32 min, max;
						PvAttrRangeUint32(cHandle, "ExposureValue", &min, &max);
						LOG(LWARNING) << "ExposureValue : "
								<< props.exposureValue
								<< " is out of range, valid range [ "
								<< (double) min / 1000000.0 << " , "
								<< (double) max / 1000000.0 << " ]\n";
					}
				}
			}
		} else {
			LOG(LWARNING) << "Unable to set ExposureMode \n";
		}
	}
	/// Gain
	if (!props.gainMode.empty()) {
		if ((err = PvAttrEnumSet(cHandle, "GainMode", props.gainMode.c_str()))
				== ePvErrSuccess) {
			if (props.gainMode == "Manual") {
				if ((err = PvAttrUint32Set(cHandle, "gainValue",
						props.gainValue)) != ePvErrSuccess) {
					if (err == ePvErrOutOfRange) {
						tPvUint32 min, max;
						PvAttrRangeUint32(cHandle, "GainValue", &min, &max);
						LOG(LWARNING) << "GainValue : " << props.gainValue
								<< " is out of range, valid range [ "
								<< (double) min << " , " << (double) max
								<< " ]\n";
					}
				}
			}
		} else {
			LOG(LWARNING) << "Unable to set GainMode \n";
		}
	}
	///	White Balance
	if (!props.whitebalMode.empty()) {
		if ((err = PvAttrEnumSet(cHandle, "WhitebalMode",
				props.gainMode.c_str())) == ePvErrSuccess) {
			if (props.whitebalMode == "Manual") {
				if ((err = PvAttrUint32Set(cHandle, "WhitebalValueRed",
						props.whitebalValueRed)) != ePvErrSuccess) {
					if (err == ePvErrOutOfRange) {
						tPvUint32 min, max;
						PvAttrRangeUint32(cHandle, "WhitebalValueRed", &min,
								&max);
						LOG(LWARNING) << "WhitebalValueRed : "
								<< props.whitebalValueRed
								<< " is out of range, valid range [ "
								<< (double) min << " , " << (double) max
								<< " ]\n";
					}
				}

				if ((err = PvAttrUint32Set(cHandle, "WhitebalValueBlue",
						props.whitebalValueBlue)) != ePvErrSuccess) {
					if (err == ePvErrOutOfRange) {
						tPvUint32 min, max;
						PvAttrRangeUint32(cHandle, "WhitebalValueBlue", &min,
								&max);
						LOG(LWARNING) << "WhitebalValueBlue : "
								<< props.whitebalValueBlue
								<< " is out of range, valid range [ "
								<< (double) min << " , " << (double) max
								<< " ]\n";
					}
				}
			}
		} else {
			LOG(LWARNING) << "Unable to set WhitebalMode" << err << "\n";
		}
	}

	if ((err = PvAttrEnumSet(cHandle, "MirrorX", props.mirrorX ? "On" : "Off"))
			!= ePvErrSuccess) {

	}

	if ((err = PvAttrEnumSet(cHandle, "PixelFormat", props.pixelFormat.c_str()))
			!= ePvErrSuccess) {
		LOG(LERROR) << "Unable to set pixelformat " << err;
	}

	if ((err = PvAttrUint32Set(cHandle, "Height", props.height))
			!= ePvErrSuccess) {
		if (err == ePvErrOutOfRange) {
			tPvUint32 min, max;
			PvAttrRangeUint32(cHandle, "Height", &min, &max);
			LOG(LWARNING) << "Height : " << props.height
					<< " is out of range, valid range [ " << (double) min
					<< " , " << (double) max << " ]";
		}
	}

	if ((err = PvAttrUint32Set(cHandle, "Width", props.width)) != ePvErrSuccess) {
		if (err == ePvErrOutOfRange) {
			tPvUint32 min, max;
			PvAttrRangeUint32(cHandle, "Width", &min, &max);
			LOG(LWARNING) << "Width : " << props.width
					<< " is out of range, valid range [ " << (double) min
					<< " , " << (double) max << " ]\n";
		}
	}

	if ((err = PvAttrUint32Set(cHandle, "RegionX", props.regionX))
			!= ePvErrSuccess) {
		if (err == ePvErrOutOfRange) {
			tPvUint32 min, max;
			PvAttrRangeUint32(cHandle, "RegionX", &min, &max);
			LOG(LWARNING) << "RegionX : " << props.regionX
					<< " is out of range, valid range [ " << (double) min
					<< " , " << (double) max << " ]\n";
		}
	}

	if ((err = PvAttrUint32Set(cHandle, "RegionY", props.regionY))
			!= ePvErrSuccess) {
		if (err == ePvErrOutOfRange) {
			tPvUint32 min, max;
			PvAttrRangeUint32(cHandle, "RegionY", &min, &max);
			LOG(LWARNING) << "RegionY : " << props.regionY
					<< " is out of range, valid range [ " << (double) min
					<< " , " << (double) max << " ]\n";
		}
	}

	if ((err = PvAttrUint32Set(cHandle, "BinningX", props.binningX))
			!= ePvErrSuccess) {
		if (err == ePvErrOutOfRange) {
			tPvUint32 min, max;
			PvAttrRangeUint32(cHandle, "BinningX", &min, &max);
			LOG(LWARNING) << "BinningX : " << props.binningX
					<< " is out of range, valid range [ " << (double) min
					<< " , " << (double) max << " ]\n";
		}
	}

	if ((err = PvAttrUint32Set(cHandle, "BinningY", props.binningY))
			!= ePvErrSuccess) {
		if (err == ePvErrOutOfRange) {
			tPvUint32 min, max;
			PvAttrRangeUint32(cHandle, "BinningY", &min, &max);
			LOG(LWARNING) << "BinningY : " << props.binningY
					<< " is out of range, valid range [ " << (double) min
					<< " , " << (double) max << " ]\n";
		}
	}
	// ----------------

	PvAttrEnumSet(cHandle, "FrameStartTriggerMode", "Freerun");

	unsigned long frameSize = 0;

	if (PvAttrUint32Get(cHandle, "TotalBytesPerFrame", &frameSize)
			!= ePvErrSuccess) {
		return false;
	}

	frame.ImageBuffer = new char[frameSize];
	frame.ImageBufferSize = frameSize;

	return true;
}

bool CameraGigE::onFinish() {
	LOG(LTRACE) << "CameraGigE::finish\n";
	PvCameraClose(cHandle);
	return true;
}

bool CameraGigE::onStep() {
	LOG(LTRACE) << "CameraGigE::onStep";

	tPvErr Err = PvCaptureQueueFrame(cHandle, &frame, NULL);
	if (!Err) {
		Err = PvCaptureWaitForFrameDone(cHandle, &frame, PVINFINITE);
		if (!Err) {
			if (frame.Status == ePvErrSuccess) {
				img = cv::Mat(frame.Height, frame.Width, (frame.Format
						== ePvFmtMono8) ? CV_8UC1 : CV_8UC3, frame.ImageBuffer);

				out_img.write(img);
				newImage->raise();
			}
		}
	}

	return true;
}

bool CameraGigE::onStart() {
	// set the camera is acquisition mode
	if (!PvCaptureStart(cHandle)) {
		// start the acquisition and make sure the trigger mode is "freerun"
		if (PvCommandRun(cHandle, "AcquisitionStart")) {
			// if that fail, we reset the camera to non capture mode
			PvCaptureEnd(cHandle);
			return false;
		} else
			return true;
	} else
		return false;
}

bool CameraGigE::onStop() {
	PvCommandRun(cHandle, "AcquisitionStop");
	PvCaptureEnd(cHandle);
	return true;
}

void CameraGigE::onTrigger() {

}

}//: namespace Sequence
}//: namespace Sources
