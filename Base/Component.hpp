/*!
 * \file Component.hpp
 * \brief Abstract interface for all components.
 *
 * \author mstefanc
 * \date 2010-04-29
 */

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <map>
#include <string>

#include <boost/foreach.hpp>

#include "Event.hpp"
#include "EventHandler.hpp"
#include "DataStreamInterface.hpp"
#include "Logger.hpp"
#include "Timer.hpp"
#include "Property.hpp"

namespace Base {

class Props;

/*!
 * \class Component

 * \~english
 * \brief Abstract interface class for all modules - data sources, processors etc.
 *
 * Every component should derive from this class, and override at least three methods:
 * onInit, onFinish, onStart, onStop and onStep.
 *
 * For more information about creating components, see \ref dev_components "developing components"
 * in \ref tutorials section.
 *
 * \~polish
 * \brief Klasa interfejsowa dla wszystkich komponentów - źródeł danych, procesorów itp.
 *
 * Każdy komponent powinien dziedziczyć po tej klasie i implementować kilka wymaganych metod
 * (onInit, onFinish, onStart, onStop oraz onStep).
 *
 * \~
 * \author mstefanc
 * \date 2010-04-29
 */
class Component
{
	typedef std::pair<std::string, Event *> EventPair;
	typedef std::pair<std::string, EventHandlerInterface *> HandlerPair;
	typedef std::pair<std::string, DataStreamInterface *> StreamPair;

public:
	/*!
	 * \~english
	 * Components state
	 * \~polish
	 * Stan komponentu
	 */
	enum State {
		Running,         ///< \~english Component is running                         \~polish Komponent jest uruchomiony
		Ready,           ///< \~english Component is stopped, ready to run           \~polish Komponent zatrzymany, gotowy do uruchomienia
		Unready          ///< \~english Component hasn't been initialized or has been finished \~polish Komponent nie został jeszcze zainicjowany albo został już zakończony
	};

	/*!
	 * \~english
	 * Base constructor
	 * \~polish
	 * Bazowy konstruktor
	 */
	Component(const std::string & n) : name_(n), state(Unready)
	{

	}

	void setName(const std::string & n) {
		name_ = n;
	}

	std::string name() {
		return name_;
	}

	/*!
	 * Virtual destructor
	 */
	virtual ~Component()
	{
		// delete all events and handlers
		BOOST_FOREACH(EventPair event, events) {
			delete event.second;
		}
	}

	/*!
	 * \~english
	 * Initialize component. For example for sources it would be opening streams or devices.
	 * \~polish
	 * Zainicjuj komponent.
	 */
	bool initialize() {
		if (state == Unready) {
			if (onInit()) {
				state = Ready;
				return true;
			} else {
				return false;
			}
		}

		if (state == Ready) {
			LOG(WARNING) << name_ << " already initialized.\n";
			return true;
		}

		if (state == Running) {
			LOG(WARNING) << name_ << " already initialized and running.\n";
			return true;
		}

		return false;
	}

	/*!
	 * Start component
	 */
	bool start() {
		if (state == Ready) {
			if (onStart()) {
				state = Running;
				return true;
			} else {
				return false;
			}
		}

		if (state == Running) {
			LOG(WARNING) << name_ << " already running.\n";
			return true;
		}

		if (state == Unready) {
			LOG(WARNING) << name_ << " is not ready to run.\n";
			return false;
		}

		return false;
	}

	/*!
	 * Stop component
	 */
	bool stop() {
		if (state == Running) {
			if (onStop()) {
				state = Ready;
				return true;
			} else {
				return false;
			}
		}

		if (state == Ready) {
			LOG(WARNING) << name_ << " already stopped.\n";
			return true;
		}

		if (state == Unready) {
			LOG(WARNING) << name_ << " is not initialized.\n";
			return false;
		}

		return false;
	}

	/*!
	 * Finish component work. Here all resources should be released.
	 */
	bool finish() {
		if (state == Ready) {
			if (onFinish()) {
				state = Unready;
				return true;
			} else {
				return false;
			}
		}

		if (state == Unready) {
			LOG(WARNING) << name_ << " is already finished.\n";
			return true;
		}

		if (state == Running) {
			LOG(WARNING) << name_ << " still running. Trying to stop...\n";
			if (stop())
				LOG(WARNING) << name_ << " stopped. Finishing...\n";
			else
				LOG(WARNING) << name_ << " didn't stop. Finishing anyway...\n";

			onFinish();

			return false;
		}

		return false;
	}

	/*!
	 * Single work step. For example sources would retrieve single frame,
	 * processors would process one bunch of data.
	 * Method called by components owner.
	 * \return execution time
	 */
	double step() {
		Common::Timer timer;
		if (state == Running) {
			timer.restart();
			onStep();
			return timer.elapsed();
		} else {
			LOG(WARNING) << name_ << " is not running. Step can't be done.\n";
			return 0;
		}
	}

	/*!
	 * Print list of all registered events.
	 */
	void printEvents() {
		LOG(INFO) << "Registered events:\n";
		BOOST_FOREACH(EventPair event, events) {
			LOG(INFO) << "\t" << event.first << "\n";
		}
	}

	/*!
	 * Returns event with specified name if registered or NULL.
	 * \param name event name
	 * \returns pointer to event with specified name or NULL if no such event is registered.
	 */
	Event * getEvent(const std::string& name) {
		if (events.count(name) > 0) {
			return events[name];
		} else {
			return NULL;
		}
	}

	/*!
	 * Print list of all registered event handlers.
	 */
	void printHandlers() {
		LOG(INFO) << "Registered handlers:\n";
		BOOST_FOREACH(HandlerPair handler, handlers) {
			LOG(INFO) << "\t" << handler.first << "\n";
		}
	}

	/*!
	 * Returns event handler with specified name if registered or NULL.
	 * \param name event handler name
	 * \returns pointer to event handler with specified name or NULL if no such event is registered.
	 */
	EventHandlerInterface * getHandler(const std::string& name) {
		if (handlers.count(name) > 0) {
			return handlers[name];
		} else {
			return NULL;
		}
	}

	/*!
	 * Print list of all registered data streams.
	 */
	void printStreams() {
		LOG(INFO) << "Registered data streams:\n";
		BOOST_FOREACH(StreamPair stream, streams) {
			LOG(INFO) << "\t" << stream.first << "\n";
		}
	}

	/*!
	 * Returns data stream with specified name if registered or NULL.
	 * \param name data stream name
	 * \returns pointer to data stream with specified name or NULL if no such stream is registered.
	 */
	DataStreamInterface * getStream(const std::string& name) {
		if (streams.count(name) > 0) {
			return streams[name];
		} else {
			return NULL;
		}
	}

	/*!
	 * Return pointer to properties of this object.
	 *
	 * Should be overridden in derived classes containing specific properties.
	 */
	virtual Props * getProperties() {
		// by default return NULL indicating, that given component has no properties
		return NULL;
	}

	/*!
	 * Check, if component is running
	 */
	bool running() {
		return state == State::Running;
	}

	/*!
	 * Check, if component is initialized
	 */
	bool initialized() {
		return state == State::Ready;
	}

protected:
	/*!
	 * Method called when component is started
	 * \return true on success
	 */
	virtual bool onStart() = 0;

	/*!
	 * Method called when component is stopped
	 * \return true on success
	 */
	virtual bool onStop() = 0;

	/*!
	 * Method called when component is initialized
	 * \return true on success
	 */
	virtual bool onInit() = 0;

	/*!
	 * Method called when component is finished
	 * \return true on success
	 */
	virtual bool onFinish() = 0;

	/*!
	 * Method called when step is called
	 * \return true on success
	 */
	virtual bool onStep() = 0;


	/*!
	 * Register new event under specified name.
	 * \param name event name
	 * \returns pointer to newly created event.
	 */
	Event * registerEvent(const std::string& name) {
		/// \todo check, if event already exists
		Event * event = new Event();
		events[name] = event;
		return event;
	}

	/*!
	 * Register new event handler under specified name.
	 * \param name event handler name
	 * \param handler pointer to proper handler
	 * \returns pointer to handler.
	 */
	EventHandlerInterface * registerHandler(const std::string& name, EventHandlerInterface * handler) {
		/// \todo check, if handler already exists
		handlers[name] = handler;
		return handler;
	}

	/*!
	 * Register new data stream under specified name.
	 * \param name stream name
	 * \param stream pointer to proper stream
	 * \returns pointer to handler.
	 */
	DataStreamInterface * registerStream(const std::string& name, DataStreamInterface * stream) {
		/// \todo check, if handler already exists
		streams[name] = stream;
		return stream;
	}

private:
	/// name of particular object
	std::string name_;

	/// state of component
	State state;

	/// all registered events
	std::map<std::string, Event *> events;

	/// all registered event handlers
	std::map<std::string, EventHandlerInterface *> handlers;

	/// all registered data streams
	std::map<std::string, DataStreamInterface *> streams;

	/// all registered properties
	std::map<std::string, PropertyInterface *> properties;

};

}//: namespace Base


#endif /* COMPONENT_HPP_ */
