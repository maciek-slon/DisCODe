/*!
 * \file SharedLibraryInc.hpp
 * \brief Include file for SharedLibrary, used by Boost::Preprocessor
 *
 * It's used to generate specified number of template methods get (with
 * variable number of arguments).
 *
 * Based on Boost::Extension library.
 *
 * \date 2010-05-07
 * \author mstefanc
 */

/*
 * Boost.Extension / implementation header for Boost.PreProcessor
 *
 * (C) Copyright Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */
# define N BOOST_PP_ITERATION()
// No ifndef headers - this is meant to be included multiple times.
template <class ReturnValue
          BOOST_PP_COMMA_IF(N)
          BOOST_PP_ENUM_PARAMS(N, class Param) >
ReturnValue (*get(const std::string& name))(BOOST_PP_ENUM_PARAMS(N, Param)) {
	// Cast the handle or pointer to the function to the correct type.
	// This is NOT typesafe.
	return reinterpret_cast<ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))>(dlsym(handle, name.c_str()));
}
#undef N
