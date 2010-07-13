/*!
 * \file CameraV4L_Panel.cc
 * \brief
 *
 * \author  tkornuta
 * \date Nov 12, 2009
 */

#include "CameraV4L_Panel.h"
#include <sstream>
#include <string>
using namespace std;

namespace Core {

namespace Sources {

CameraV4L_Panel::CameraV4L_Panel() :
	Base::EventPanel("CameraV4L_Panel", "/settings/sources/CameraV4L", 0, 145,
			600, 180, "CameraV4L management") {
}

CameraV4L_Panel::~CameraV4L_Panel() {

}

/*
 * funkcje bezklasowe
 */
const string Int2Char(int i) {
	string s;
	stringstream out;
	out << i;
	s = out.str();
	return s;
}

vector<string> splitString(string value_) {
	vector<string> atributes;
	/*
	 * prosty parser serializowanych danych
	 */
	char * pch;
	pch = strtok((char *) value_.c_str(), ";");
	while (pch != NULL) {
		atributes.push_back(string(pch));
		pch = strtok(NULL, ";");
	}
	return atributes;
}

bool CameraV4L_Panel::handleSingleEvent(string attribute_, string value_) {

	if (attribute_.compare("startSettings") == 0 or attribute_.compare(
			"restartSettings_io") or attribute_.compare("restartSettings_dev")
			== 0) {
		vector<string> atr = splitString(value_);
		vector<string>::iterator it;
		int whichCase = 0;
		for (it = atr.begin(); it != atr.end(); it++) {
			if (it->compare("Devices") == 0) {
				whichCase = 1;
				continue;
			}
			if (it->compare("Channels") == 0) {
				whichCase = 2;
				continue;
			}
			if (it->compare("Palettes") == 0) {
				whichCase = 3;
				continue;
			}
			if (it->compare("Image_size") == 0) {
				whichCase = 4;
				continue;
			}
			/*if(it->compare("Height")==0)
			 {
			 whichCase = 5;
			 continue;
			 }*/
			if (it->compare("Standard") == 0) {
				whichCase = 6;
				continue;
			}
			if (it->compare("Interlace") == 0) {
				whichCase = 7;
				continue;
			}
			if (it->compare("IOMethod") == 0) {
				whichCase = 8;
				continue;
			}

			switch (whichCase) {
			case 1:
				ch_dev_name->add(it->c_str());
				break;
			case 2:
				ch_channel->add(it->c_str());
				break;
				/*	case 3:
				 ch_palette->add(it->c_str());
				 break;*/
			case 4:
				ch_image_size->add(it->c_str());
				break;
			case 5:
				//	ch_height->add(it->c_str());
				break;
			case 6:
				ch_standard->add(it->c_str());
				break;
			case 7:
				ch_interlace->add(it->c_str());
				break;
			case 8:
				ch_io_method->add(it->c_str());
				break;
			default:
				;
			}

		}
	}
	if (attribute_.compare("restartSettings_io") == 0)
		loadConfigurationFromXML("restartSettings_io");
	if (attribute_.compare("restartSettings_dev") == 0)
		loadConfigurationFromXML("restartSettings_dev");

	if (attribute_.compare("Image_size") == 0) {
		ch_image_size->add(value_.c_str());
		setChoiceItem(ch_image_size, value_.c_str());
	}
	/*
	 if (attribute_.compare("height") == 0){
	 ch_height->add(value_.c_str());
	 setChoiceItem(ch_height, value_.c_str());
	 }
	 */
	return true;
}

bool CameraV4L_Panel::loadConfigurationFromXML(string restart_) {
	/*
	 * frame grabber
	 */
	string device = getConfigValue("input_device", "device");
	string video_input = getConfigValue("input_device", "video_input");
	string video_standard = getConfigValue("input_device", "video_standard");
	string io_method = getConfigValue("input_device", "io_method");
	string interlace = getConfigValue("input_device", "interlace");
	/*
	 * window
	 */
	string width = (getConfigValue("picture_settings", "width")).c_str();
	string height = (getConfigValue("picture_settings", "height")).c_str();
	string image_size = width + "x" + height;
	string palette = getConfigValue("picture_settings", "palette");

	/*
	 * picture
	 */
	int brightness = atoi(
			(getConfigValue("picture_settings", "brightness")).c_str());
	int whiteness = atoi(
			(getConfigValue("picture_settings", "whiteness")).c_str());
	int contrast = atoi(
			(getConfigValue("picture_settings", "contrast")).c_str());
	int hue = atoi((getConfigValue("picture_settings", "hue")).c_str());

	if (restart_.compare("restartSettings_dev") != 0)
		if (!setChoiceItem(ch_dev_name, device.c_str())) {
			ch_dev_name->add(device.c_str());
			setChoiceItem(ch_dev_name, device.c_str());
		}

	if (!setChoiceItem(ch_channel, video_input.c_str())) {
		ch_channel->add(video_input.c_str());
		setChoiceItem(ch_channel, video_input.c_str());
	}

	if (!setChoiceItem(ch_standard, video_standard.c_str())) {
		ch_standard->add(video_standard.c_str());
		setChoiceItem(ch_standard, video_standard.c_str());
	}

	if (restart_.compare("restartSettings_io") != 0)
		if (!setChoiceItem(ch_io_method, io_method.c_str())) {
			ch_io_method->add(io_method.c_str());
			setChoiceItem(ch_io_method, io_method.c_str());
		}

	if (!setChoiceItem(ch_interlace, interlace.c_str())) {
		ch_interlace->add(interlace.c_str());
		setChoiceItem(ch_interlace, interlace.c_str());
	}

	if (!setChoiceItem(ch_image_size, image_size.c_str())) {
		ch_image_size->add(image_size.c_str());
		setChoiceItem(ch_image_size, image_size.c_str());
	}
	/*
	 if (!setChoiceItem(ch_width,Int2Char(width).c_str())) {
	 ch_width->add(Int2Char(width).c_str());
	 setChoiceItem(ch_width,Int2Char(width).c_str());
	 }
	 */
	/*		if (!setChoiceItem(ch_palette,palette.c_str())) {
	 ch_palette->add(palette.c_str());
	 setChoiceItem(ch_palette,palette.c_str());
	 }
	 */
	((Fl_Valuator*) sc_brightness)->value(brightness);

	((Fl_Valuator*) sc_whiteness)->value(whiteness);

	((Fl_Valuator*) sc_hue)->value(hue);

	((Fl_Valuator*) sc_contrast)->value(contrast);

	// Rest is not required - f movie will be loaded correctly, then an event from source will be sent.
	return true;
}

bool CameraV4L_Panel::loadConfiguration() {
	loadConfigurationFromXML("loadConfiguration");
	return true;
}

bool CameraV4L_Panel::saveConfiguration() {
	// Return true if configuration was saved properly.
	return true;
}

void CameraV4L_Panel::createContent() {

	ch_dev_name = new Fl_Choice(20, 30 + top, 120, 25, "");
	ch_dev_name->callback(cb_device, (void*) this);
	new Fl_Text_Display(70, 75 + top, 0, 0, "Device");

	ch_io_method = new Fl_Choice(160, 30 + top, 120, 25, "");
	ch_io_method->callback(cb_io_method, (void*) this);
	new Fl_Text_Display(210, 75 + top, 0, 0, "IO Method");

	ch_interlace = new Fl_Choice(300, 30 + top, 120, 25, "");
	ch_interlace->callback(cb_interlace, (void*) this);
	new Fl_Text_Display(350, 75 + top, 0, 0, "Interlace");

	sc_brightness = new Fl_Scrollbar(20, 80 + top, 120, 20, "Brightness");
	sc_brightness->type(FL_HORIZONTAL);
	sc_brightness->slider_size(.1);
	sc_brightness->bounds(0, 100);
	sc_brightness->step(5);
	sc_brightness->callback(cb_brightness, (void*) this);

	sc_whiteness = new Fl_Scrollbar(160, 80 + top, 120, 20, "Whiteness");
	sc_whiteness->type(FL_HORIZONTAL);
	sc_whiteness->slider_size(.1);
	sc_whiteness->bounds(0, 100);
	sc_whiteness->step(5);
	sc_whiteness->callback(cb_whiteness, (void*) this);

	sc_contrast = new Fl_Scrollbar(300, 80 + top, 120, 20, "Contrast");
	sc_contrast->type(FL_HORIZONTAL);
	sc_contrast->slider_size(.1);
	sc_contrast->bounds(0, 100);
	sc_contrast->step(5);
	sc_contrast->callback(cb_contrast, (void*) this);

	sc_hue = new Fl_Scrollbar(440, 80 + top, 120, 20, "Hue");
	sc_hue->type(FL_HORIZONTAL);
	sc_hue->slider_size(.1);
	sc_hue->bounds(0, 100);
	sc_hue->step(5);
	sc_hue->callback(cb_hue, (void*) this);

	ch_channel = new Fl_Choice(20, 130 + top, 120, 25, "");
	ch_channel->callback(cb_channel, (void*) this);
	new Fl_Text_Display(70, 180 + top, 0, 0, "Channel");

	ch_standard = new Fl_Choice(160, 130 + top, 120, 25, "");
	ch_standard->callback(cb_video_standard, (void*) this);
	new Fl_Text_Display(200, 180 + top, 0, 0, "Standard");

	ch_image_size = new Fl_Choice(300, 130 + top, 120, 25, "");
	ch_image_size->callback(cb_image_size, (void*) this);
	new Fl_Text_Display(350, 180 + top, 0, 0, "Image size");
	/*
	 ch_width = new Fl_Choice(365, 130 + top , 120, 25 ,"");
	 ch_width->callback(cb_width, (void*)this);
	 new Fl_Text_Display(420, 180+top, 0,0 , "Width");*/

	/*	ch_palette = new Fl_Choice(440, 130+ top , 120, 25 ,"");
	 ch_palette->callback(cb_palette, (void*)this);
	 new Fl_Text_Display(480, 180 + top, 0,0, "Palette");
	 */
}

/*!
 * Callback - change brightness.
 */
void CameraV4L_Panel::cb_brightness(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	int brightness = ((Fl_Scrollbar*) w_)->value();
	xmlds->raiseEvent("CameraV4L_Source", "brightness", Int2Char(brightness));
}

/*!
 * Callback - change whiteness.
 */
void CameraV4L_Panel::cb_whiteness(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	int whiteness = ((Fl_Scrollbar*) w_)->value();
	xmlds->raiseEvent("CameraV4L_Source", "whiteness", Int2Char(whiteness));
}

/*!
 * Callback - change contrast.
 */
void CameraV4L_Panel::cb_contrast(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	int contrast = ((Fl_Scrollbar*) w_)->value();
	xmlds->raiseEvent("CameraV4L_Source", "contrast", Int2Char(contrast));
}

/*!
 * Callback - change hue.
 */
void CameraV4L_Panel::cb_hue(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	int hue = ((Fl_Scrollbar*) w_)->value();
	xmlds->raiseEvent("CameraV4L_Source", "hue", Int2Char(hue));
}

/*!
 * Callback - change channel.
 */
void CameraV4L_Panel::cb_channel(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char * s_channel = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "video_input", s_channel);
}

/*!
 * Callback - change palette.
 */
void CameraV4L_Panel::cb_palette(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char * s_pannel = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "palette", s_pannel);
}

/*!
 * Callback - change video standard.
 */
void CameraV4L_Panel::cb_video_standard(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char *s_standard = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "video_standard", s_standard);
}

/*!
 * Callback - change window size.
 */
void CameraV4L_Panel::cb_width(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char * c_width = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "width", c_width);
}

void CameraV4L_Panel::cb_height(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char * c_height = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "height", c_height);
}

/*!
 * Callback - change device.
 */
void CameraV4L_Panel::cb_device(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char * device = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "device", device);
}

void CameraV4L_Panel::cb_io_method(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char * io_method = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "io_method", io_method);
}

void CameraV4L_Panel::cb_interlace(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char * interlace = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "interlace", interlace);
}

void CameraV4L_Panel::cb_image_size(Fl_Widget* w_, void* data_) {
	XMLDataSynchronizer* xmlds = XMLDataSynchronizer::getInstance();
	const char * image_size = ((Fl_Choice*) w_)->text();
	xmlds->raiseEvent("CameraV4L_Source", "image_size", image_size);
}

}

}

