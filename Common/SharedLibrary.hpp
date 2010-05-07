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
 * \brief
 *
 * \author mstefanc
 */
class SharedLibrary {
public:
	SharedLibrary() : handle(0), auto_close(false) {};

	SharedLibrary(const std::string & fname, bool auto_cl) :
		handle(0), auto_close(auto_cl), location(fname) {};

	~SharedLibrary() {
		if (handle && auto_close)
			dlclose(handle);
	}

	bool setLocation(const std::string& fname, bool auto_open = false) {
		location = fname;
		if (auto_open) {
			return load();
		} else {
			return true;
		}
	}

	bool load() {
		if (location == "")
			return false;

		if (handle && !unload())
			return false;

		handle = dlopen(location.c_str(), RTLD_LAZY);

		return (handle != 0);
	}

	bool loaded() {
		return (handle != 0);
	}

	bool unload() {
		return (!dlclose(handle));
	}

	char * error() {
		return dlerror();
	}

	// If Doxygen is being run, use more readable definitions for the documentation.
	#ifdef DOXYGEN_INVOKED
		/** \brief Get a function reference.
		 *
		 * A templated function taking as template arguments the
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
		 * \warning If the function signature does not match, strange errors
		 * can occur.
		 * \pre is_open() == true.
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
	library_handle handle;
	bool auto_close;
	std::string location;
};

} //: namespace Common

#endif /* SHAREDLIBRARY_HPP_ */
