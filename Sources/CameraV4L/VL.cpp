#include "VL.hpp"
#include "V4L.hpp"
#include "V4L2.hpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include <dirent.h>
#include <vector>
using namespace std;
using std::string;

#include "boost/filesystem.hpp"
using namespace boost::filesystem;

#include "Utils.hpp"

namespace Sources {
namespace CameraV4L {

vector<string> VL::getDevices() {
	return Utils::searchFiles("/dev", "video\\d*", false);
}

vector<string> VL::getWidth() {
	vector<string> tmp;
	tmp.push_back("640");
	tmp.push_back("320");
	tmp.push_back("160");
	tmp.push_back("768");
	return tmp;
}

vector<string> VL::getHeight() {
	vector<string> tmp;
	tmp.push_back("480");
	tmp.push_back("240");
	tmp.push_back("120");
	tmp.push_back("576");
	return tmp;
}

vector<string> VL::getImageSize() {
	vector<string> tmp;
	tmp.push_back("640x480");
	tmp.push_back("320x240");
	tmp.push_back("160x120");
	tmp.push_back("768x576");
	return tmp;
}

vector<string> VL::getStandard() {
	vector<string> tmp;
	tmp.push_back("PAL");
	tmp.push_back("NTSC");
	tmp.push_back("SECAM");
	return tmp;
}

int VL::convPalette(string palette) {

	if (palette.compare("RGB") == 0)
		return RGB24;
	if (palette.compare("BGR") == 0)
		return RGB24;
	if (palette.compare("YUYV") == 0)
		return YUYV;
	if (palette.compare("UYVY") == 0)
		return UYVY;
	if (palette.compare("RGB24_2") == 0)
		return RGB24_2;
	if (palette.compare("BGR24_2") == 0)
		return BGR24_2;
	if (palette.compare("YUYV_2") == 0)
		return YUYV_2;
	if (palette.compare("UYVY_2") == 0)
		return UYVY_2;
	if (palette.compare("GREY_2") == 0)
		return V4L2_PIX_FMT_GREY;
	if (palette.compare("HI240_2") == 0)
		return V4L2_PIX_FMT_HI240;
	if (palette.compare("RGB332_2") == 0)
		return V4L2_PIX_FMT_RGB332;
	if (palette.compare("YVU410_2") == 0)
		return V4L2_PIX_FMT_YVU410;
	if (palette.compare("YUV410_2") == 0)
		return V4L2_PIX_FMT_YUV410;
	if (palette.compare("YUV411P_2") == 0)
		return V4L2_PIX_FMT_YUV411P;
	if (palette.compare("NV12_2") == 0)
		return V4L2_PIX_FMT_NV12;
	if (palette.compare("NV21_2") == 0)
		return V4L2_PIX_FMT_NV21;
	if (palette.compare("Y41P_2") == 0)
		return V4L2_PIX_FMT_Y41P;
	if (palette.compare("YVU420_2") == 0)
		return V4L2_PIX_FMT_YVU420;
	if (palette.compare("YUV420_2") == 0)
		return V4L2_PIX_FMT_YUV420;
	if (palette.compare("YUV422P_2") == 0)
		return V4L2_PIX_FMT_YUV422P;
	if (palette.compare("YYUV_2") == 0)
		return V4L2_PIX_FMT_YYUV;
	if (palette.compare("RGB555_2") == 0)
		return V4L2_PIX_FMT_RGB555;
	if (palette.compare("RGB565_2") == 0)
		return V4L2_PIX_FMT_RGB565;
	if (palette.compare("RGB555X_2") == 0)
		return V4L2_PIX_FMT_RGB555X;
	if (palette.compare("RGB565X_2") == 0)
		return V4L2_PIX_FMT_RGB565X;
	if (palette.compare("BGR32_2") == 0)
		return V4L2_PIX_FMT_BGR32;
	if (palette.compare("RGB32_2") == 0)
		return V4L2_PIX_FMT_RGB32;

	return YUYV_2;
}
/*
 * Method converts string to int
 */
int VL::convChannel(string palette) {
	vector<string> channels = getChannels();
	vector<string>::iterator it;
	int i_chan = 0;
	for (it = channels.begin(); it != channels.end(); it++) {
		if (it->compare(palette) == 0)
			return i_chan;
		i_chan++;
	}
	return 0;
}
/*
 * Method converts string to int
 */
int VL::convStandard(string palette) {
	if (palette == "PAL")
		return 0;
	else if (palette == "NTSC")
		return 1;
	else if (palette == "SECAM")
		return 2;
	else
		return -1;
}

io_method VL::convIOMethod(string method) {
	if (method.compare("MMAP") == 0)
		return IO_METHOD_MMAP;
	if (method.compare("READ") == 0)
		return IO_METHOD_READ;
	if (method.compare("USRPTR") == 0)
		return IO_METHOD_USERPTR;

	/// \todo default return?
	return IO_METHOD_MMAP;
}

vector<string> VL::getInterlace() {
	vector<string> tmp;
	tmp.push_back("ANY");
	tmp.push_back("NONE");
	tmp.push_back("TOP");
	tmp.push_back("BOTTOM");
	tmp.push_back("INTERLACED");
	tmp.push_back("SEQ_TB");
	tmp.push_back("SEQ_BT");
	tmp.push_back("ALTERNATE");
	tmp.push_back("INTERLACED_TB");
	tmp.push_back("INTERLACED_BT");
	return tmp;
}

v4l2_field VL::convInterlace(int i) {

	switch (i) {
	case 0:
		return V4L2_FIELD_ANY;
		break;
	case 1:
		return V4L2_FIELD_NONE;
		break;
	case 2:
		return V4L2_FIELD_TOP;
		break;
	case 3:
		return V4L2_FIELD_BOTTOM;
		break;
	case 4:
		return V4L2_FIELD_INTERLACED;
		break;
	case 5:
		return V4L2_FIELD_SEQ_TB;
		break;
	case 6:
		return V4L2_FIELD_SEQ_BT;
		break;
	case 7:
		return V4L2_FIELD_ALTERNATE;
		break;
	case 8:
		return V4L2_FIELD_INTERLACED_TB;
		break;
	case 9:
		return V4L2_FIELD_INTERLACED_BT;
		break;
	default:
		return V4L2_FIELD_BOTTOM;
	}
}

int VL::convInterlace2String(string i) {

	if (i.compare("ANY") == 0)
		return 0;
	if (i.compare("NONE") == 0)
		return 1;
	if (i.compare("TOP") == 0)
		return 2;
	if (i.compare("BOTTOM") == 0)
		return 3;
	if (i.compare("INTERLACED") == 0)
		return 4;
	if (i.compare("SEQ_TB") == 0)
		return 5;
	if (i.compare("SEQ_BT") == 0)
		return 6;
	if (i.compare("ALTERNATE") == 0)
		return 7;
	if (i.compare("INTERLACED_TB") == 0)
		return 8;
	if (i.compare("INTERLACED_BT") == 0)
		return 9;

	return 4;
}

}
}
