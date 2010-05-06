/*
 * Boost.Extension / libraries management:
 *         low-level platform specific dynamic library management
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_LIBRARY_IMPL_HPP
#define BOOST_EXTENSION_LIBRARY_IMPL_HPP

#include <cstring>

// Windows variant
#if ( defined(_WIN32) || defined(__WIN32__) || defined(WIN32) ) && !defined(BOOST_DISABLE_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef BOOST_EXTENSION_NO_LEAN_WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#endif

#include <Windows.h>

namespace boost {
namespace extensions {
namespace impl {

typedef HMODULE library_handle;
typedef FARPROC generic_function_ptr;

/* POSIX says dlerror( ) doesn't have to be thread-safe, so we use one
 * static buffer.
 * MSDN says the buffer cannot be larger than 64K bytes, so we set it to
 * the limit.
 */
class helper {
public:
	static char error_buffer[65535];
	static char *current_error;

	static int copy_string( char *dest, int dest_size, const char *src )
	{
	    int i = 0;

	    /* gcc should optimize this out */
	    if( !src && !dest )
	        return 0;

	    for( i = 0 ; i < dest_size-1 ; i++ )
	    {
	        if( !src[i] )
	            break;
	        else
	            dest[i] = src[i];
	    }
	    dest[i] = '\0';

	    return i;
	}

	static void save_err_str( const char *str )
	{
		DWORD dwMessageId;
		DWORD pos;


		dwMessageId = GetLastError( );


		if( dwMessageId == 0 )
			return;


		/* Format error message to:
		 * "<argument to function that failed>": <Windows localized error message>
		 */
		pos  = copy_string( error_buffer,     sizeof(error_buffer),     "\"" );
		pos += copy_string( error_buffer+pos, sizeof(error_buffer)-pos, str );
		pos += copy_string( error_buffer+pos, sizeof(error_buffer)-pos, "\": " );
		pos += FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwMessageId,
							  MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
							  error_buffer+pos, sizeof(error_buffer)-pos, NULL );


		if( pos > 1 )
		{
			/* POSIX says the string must not have trailing <newline> */
			if( error_buffer[pos-2] == '\r' && error_buffer[pos-1] == '\n' )
				error_buffer[pos-2] = '\0';
		}

		current_error = error_buffer;
	}


	static void save_err_ptr_str( const void *ptr )
	{
		char ptr_buf[19]; /* 0x<pointer> up to 64 bits. */

		sprintf( ptr_buf, "0x%p", ptr );

		save_err_str( ptr_buf );
	}
};

inline library_handle load_shared_library(const char* library_name) {
	helper::current_error = NULL;

	UINT uMode;

	/* Do not let Windows display the critical-error-handler message box */
	uMode = SetErrorMode( SEM_FAILCRITICALERRORS );
	library_handle h = LoadLibraryA(library_name);
	if (!h)
		helper::save_err_str( library_name );

	SetErrorMode( uMode );

	return h;
}

inline generic_function_ptr get_function(library_handle handle, const char* function_name) {
	return GetProcAddress(handle, function_name);
}

inline bool close_shared_library(library_handle handle) {
	bool ret;

	helper::current_error = NULL;

	ret = FreeLibrary( handle );

	if (!ret)
		helper::save_err_ptr_str( handle );

	return (ret);
}

inline char * last_error() {
	return NULL;
}

}//: namespace impl
}//: namespace extensions
}//: namespace boost

#if (defined(MSVC))
#   pragma comment(lib, "kernel32.lib")
#endif

// *nix variant
#else

#include <dlfcn.h>

namespace boost {
namespace extensions {
namespace impl {

typedef void * library_handle;
typedef void * generic_function_ptr;

inline library_handle load_shared_library(const char* library_name) {
	return dlopen(library_name, RTLD_LAZY);
}

inline generic_function_ptr get_function(library_handle handle, const char* function_name) {
	return dlsym(handle, function_name);
}

inline bool close_shared_library(library_handle handle) {
	return (dlclose(handle) == 0);
}

inline char * last_error() {
	return dlerror();
}

} //: namespace impl
} //: namespace extensions
} //: namespace boost

#endif /* OS check */

#endif /* BOOST_EXTENSION_LIBRARY_IMPL_HPP */
