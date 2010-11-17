/*!
 * \file SharedLibrary.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-05-02
 */

#ifndef SHAREDLIBRARY_HPP_
#define SHAREDLIBRARY_HPP_

#include <string>
#include <dlfcn.h>

#include "SharedLibraryCommon.hpp"

namespace Common {

/*!
 * \class SharedLibrary
 * \brief Class representing shared object.
 *
 * It's responsible for loading and unloading library files and retrieving functions from them.
 *
 * \author mstefanc
 */
class SharedLibrary {
public:
	/*!
	 * SharedLibrary constructor.
	 * \param fname library filename, on some systems it should also have path included (even if it's
	 * located in the same directory as executable - then fname should be ./file.so)
	 * \param auto_cl if set to true unload will be called in destructor, if false unload must be called explicitly.
	 */
	SharedLibrary(const std::string & fname = "", bool auto_cl = true) :
		handle(0), auto_close(auto_cl), location(fname) {};

	/*!
	 * Destructor.
	 *
	 * If automatic library closing was set to true in constructor and library is loaded then destructor
	 * unloads it from memory, in other situation it does nothing.
	 */
	~SharedLibrary() {
		if (handle && auto_close)
			dlclose(handle);
	}

	/*!
	 * Set location of library.
	 *
	 * Useful if empty constructor was used during initialization.
	 * \param fname library filename, on some systems it should also have path included (even if it's
	 * located in the same directory as executable - then fname should be ./file.so)
	 * \param auto_open if set to true then library will be immediately loaded
	 * \return
	 * - false if auto_open was set but library can't be loaded
	 * - true otherwise
	 */
	bool setLocation(const std::string& fname, bool auto_open = false) {
		location = fname;
		if (auto_open) {
			return load();
		} else {
			return true;
		}
	}

	/*!
	 * Load shared library.
	 *
	 * If library is already loaded then unload is called and then library is reloaded.
	 *
	 * \return true if load was successful, false if some error occurs (invalid file name, unsuccessful
	 * library unload etc)
	 */
	bool load() {
		if (location == "")
			return false;

		if (handle && !unload())
			return false;

		handle = dlopen(location.c_str(), RTLD_LAZY);

		return (handle != 0);
	}

	/*!
	 * Check if library is loaded
	 * \return true if library is loaded, false otherwise
	 */
	bool loaded() const {
		return (handle != 0);
	}

	/*!
	 * Unload library from memory.
	 *
	 * \return true if library was unloaded, false otherwise
	 */
	bool unload() {
		if (!dlclose(handle)) {
			handle = 0;
			return true;
		} else {
			return false;
		}
	}

	/*!
	 * Return error from last call.
	 *
	 * \return pointer to textual description of last call error (if there was any) or NULL
	 * when last call was successful.
	 */
	char * error() {
		return dlerror();
	}

	// If Doxygen is being run, use more readable definitions for the documentation.
	#ifdef DOXYGEN_INVOKED
		/**
		 * \brief Get a function reference.
		 *
		 * A template function taking as template arguments the
		 * type of the return value and parameters of a function
		 * to look up in the shared library.
		 *
		 * This function must have been declared with the same
		 * parameters and return type and marked as extern "C".
		 *
		 * Depending on platform and compiler settings, it may also
		 * be necessary to prefix the function with BOOST_EXTENSION_DECL,
		 * to make it externally visible.
		 *
		 * \warning If the function signature does not match, strange errors can occur.
		 * \pre loaded() == true.
		 * \post None.
		 */
		template <class RetValue, class Params...>
		FunctionPtr<ReturnValue (Params...)>
		get(const std::string& name) const {
		}
	#else /* DOXYGEN_INVOKED */

	#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_INC(SHARED_LIBRARY_MAX_FUNCTOR_PARAMS) - 1)
	#define BOOST_PP_FILENAME_1 <SharedLibraryInc.hpp>
	#include BOOST_PP_ITERATE()

	#endif /* DOXYGEN_INVOKED */

protected:
	/// internal handle to opened library object
	library_handle handle;

	/// automatic library close flag
	bool auto_close;

	/// location of library
	std::string location;
};

} //: namespace Common

#endif /* SHAREDLIBRARY_HPP_ */
