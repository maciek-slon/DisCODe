/*!
 * \file
 * \brief
 */


#ifndef KEYBOARD_SOURCE_HPP_
#define KEYBOARD_SOURCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <map>

/**
 * \defgroup Keyboard Keyboard
 * \ingroup Sources
 * \brief Keyboard reader, raises events connected with keystrokes.
 *
 * Define active keys in 'keys' property, and use it's events. For example, with
 * keys="ABC", there are 3 events available: A_Pressed, B_Pressed and C_Pressed.
 *
 * \par Notes:
 *
 * \li it's all case sensitive, so if you specify keys="Aa" there will be two events created: A_Pressed and a_Pressed
 * \li this component have to be run in separate thread, otherwise task execution will be freezed in each step until ENTER is pressed.
 *
 *
 * \par Data streams:
 *
 * none
 *
 * \par Events:
 *
 * \event{[KEY]_Pressed}
 * [KEY] pressed; only those events which are defined by 'keys' property are available.
 *
 *
 * \par Event handlers:
 *
 * none
 *
 * \par Properties:
 *
 * \prop{keys,string,"ABC"}
 * Keys, which have associated events
 *
 * @{
 *
 * @}
 */

namespace Sources {
namespace Keyboard {

/*!
 * \class Props
 * \brief Keyboard_Source properties
 */
struct Props : public Base::Props {

	std::string keys;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt) {
		keys = pt.get("keys", "ABC");
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt) {
		pt.put("keys", keys);
	}
};

/*!
 * \class Keyboard_Source
 * \brief Class responsible for retrieving images from movies.
 */
class Keyboard_Source : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	Keyboard_Source(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~Keyboard_Source();

	/*!
	 * Return movie properties
	 */
	Base::Props * getProperties() {
		return &props;
	}

protected:
	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();


	/// Event signaling that new image was retrieved.
	std::map<char, Base::Event *> key_events;

	/// Keyboard properties
	Props props;

	bool trig;
};

}//: namespace Keyboard
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_SOURCE_COMPONENT("Keyboard", Sources::Keyboard::Keyboard_Source, Common::Panel_Empty)

#endif /* KEYBOARD_SOURCE_HPP_ */
