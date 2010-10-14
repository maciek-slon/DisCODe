/*!
 * \file Thread.cpp
 * \brief System specific routines for handling threads
 *
 * \author mstefanc
 * \date 2010-05-06
 */

#include "Thread.hpp"

namespace Common {

#ifdef WIN32 /* Windows platform */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef HANDLE thread_handle;
typedef DWORD err_core;

bool Thread::setPriority(Priority priority) {
	thread_handle th = thread.native_handle();

	if (!th)
		return false;

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

bool Thread::kill() {
	thread_handle th = thread.native_handle();
	if (!th)
		return false;

	BOOL res;

	res = TerminateThread(th, 0);
	if (res == FALSE)
		return false;
	else
		return true;
}

#else /* *nix platform */

typedef pthread_t thread_handle;
typedef int err_code;

bool Thread::setPriority(Priority priority) {
	return false;
}

bool Thread::kill() {
	thread_handle th = thread.native_handle();
	if (!th)
		return false;

	pthread_cancel(th);
	return true;
}

#endif /* OS select */

} //: namespace Common
