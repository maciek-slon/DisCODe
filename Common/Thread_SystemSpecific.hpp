/*!
 * \file Thread_SystemSpecific
 * \brief System specific routines for handling threads
 *
 * \author mstefanc
 * \date 2010-05-06
 */

#ifndef THREAD_SYSTEMSPECIFIC_HPP_
#define THREAD_SYSTEMSPECIFIC_HPP_

namespace Common {

/*!
 * Possible thread priorities
 */
enum Priority {
	tpRealtime,
	tpHigh,
	tpAbove,
	tpNormal,
	tpBelow,
	tpIdle
};

namespace Thread_SystemSpecific {

#ifdef WIN32 /* Windows platform */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef HANDLE thread_handle;
typedef DWORD err_core;

inline bool setThreadPriority(thread_handle thread, Priority priority) {
	if (!thread)
		return;

	BOOL res;

	switch (priority)
	{
	case tpRealtime:
		res = SetThreadPriority(th, THREAD_PRIORITY_TIME_CRITICAL);
		break;
	case tpHigh:
		res = SetThreadPriority(th, THREAD_PRIORITY_HIGHEST);
		break;
	case tpAbove:
		res = SetThreadPriority(th, THREAD_PRIORITY_ABOVE_NORMAL);
		break;
	case tpNormal:
		res = SetThreadPriority(th, THREAD_PRIORITY_NORMAL);
		break;
	case tpBelow:
		res = SetThreadPriority(th, THREAD_PRIORITY_BELOW_NORMAL);
		break;
	case tpIdle:
		res = SetThreadPriority(th, THREAD_PRIORITY_LOWEST);
		break;
	}

	if (res == FALSE)
		return false;
	else
		return true;
}

inline char * getLastError() {
	return NULL;
}

inline bool killThread(thread_handle thread) {
	BOOL res;
	res = TerminateThread(thread, 0);
	if (res == FALSE)
		return false;
	else
		return true;
}

#else /* *nix platform */

typedef pthread_t thread_handle;
typedef int err_code;

inline bool setThreadPriority(thread_handle thread, Priority priority) {
	return false;
}

inline char * getLastError() {
	return NULL;
}

inline bool killThread(thread_handle thread) {
	pthread_cancel(thread);
	return true;
}

#endif /* OS select */


} //: namespace Thread_SystemSpecific
} //: namespace Common

#endif /* THREAD_SYSTEMSPECIFIC_HPP_ */
