/*!
 * \file CameraV4L_Panel.h
 * \brief
 *
 * \author  tkornuta
 * \date Nov 12, 2009
 */

#ifndef CAMERAV4L_PANEL_H_
#define CAMERAV4L_PANEL_H_

#include "EventPanel.h"
#include "V4L_define.h"
#include <string>
#include <vector>
using namespace std;

namespace Core {

namespace Sources {

class CameraV4L_Panel: public Base::EventPanel {

	Fl_Choice * ch_dev_name;

    //brightness
    Fl_Scrollbar * sc_brightness;

    Fl_Scrollbar * sc_whiteness;

    Fl_Scrollbar * sc_contrast;
    //hue
    Fl_Scrollbar * sc_hue;

    //ustawienia kanalow, palety i standardu
    Fl_Choice * ch_channel;

    Fl_Choice * ch_standard ;

	Fl_Choice * ch_height;

	Fl_Choice * ch_width;

	Fl_Choice * ch_palette;

	Fl_Choice * ch_io_method;

	Fl_Choice * ch_interlace;

	Fl_Choice * ch_image_size;
	/*!
	 * Callback - change brightness.
	 */
	static void cb_brightness(Fl_Widget* w_, void* data_);


	/*!
	  * Callback - change whiteness.
	  */
	static void cb_whiteness(Fl_Widget* w_, void* data_);

	 /*!
	  * Callback - change contrast.
	  */
	static void cb_contrast(Fl_Widget* w_, void* data_);

	 /*!
	  * Callback - change hue.
	  */
	static void cb_hue(Fl_Widget* w_, void* data_);


	 /*!
	  * Callback - change channel.
	  */
	static void cb_channel(Fl_Widget* w_, void* data_);

	/*!
	 * Callback - change palette.
	 */
	static void cb_palette(Fl_Widget* w_, void* data_);


	 /*!
	  * Callback - change video standard.
	  */
	static void cb_video_standard(Fl_Widget* w_, void* data_);

	static void cb_height(Fl_Widget* w_, void* data_);

	static void cb_width(Fl_Widget* w_, void* data_);

	 /*!
	  * Callback - change device.
	  */
	static void cb_device(Fl_Widget* w_, void* data_);

	static void cb_io_method(Fl_Widget* w_, void* data_);

	static void cb_interlace(Fl_Widget* w_, void* data_);

	static void cb_image_size(Fl_Widget* w_, void* data_);

	/*!
	 * Callback - attached to load button.
	 */
//	static void cb_load(Fl_Widget*, void*);

	/*!
	 * Single event handler. Returns true if handled correctly.
	 */
	bool handleSingleEvent(string attribute_, string value_);

public:
	/*!
	 * Required definition of parametrized constructor.
	 */
	CameraV4L_Panel();

	/*!
	 * Destructor.
	 */
    virtual ~CameraV4L_Panel();

	/*!
	 * Method loads configuration from file.
	 */
	bool loadConfiguration();

	bool loadConfigurationFromXML(string restart_);

	/*!
	 * Method saves configuration to file.
	 */
	bool saveConfiguration();

	/*!
	 * Method responsible for creation of panel's content.
	 */
	void createContent();

	/*!
	 * Method responsible for initialization of default settings (DEPRICATED).
	 */
	void initializeSettings() {}

	/*!
	 * Method responsible for panel refreshment (DEPRICATED).
	 */
	void refreshPanel() {}

};

}

}

#endif /* CAMERAV4L_PANEL_H_ */

