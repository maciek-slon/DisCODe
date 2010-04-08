/*!
 * \file WindowOpenCV.cpp
 * \brief Declaration of an example image viewer.
 * \author mstefanc
 * \date 2010-03-10
 */

#ifndef WINDOWOPENCV_HPP
#define WINDOWOPENCV_HPP

#include <cv.h>
#include <highgui.h>

#include "Kernel_Aux.hpp"
#include "PanelEmpty.hpp"

/*!
 * \class WindowOpenCV
 * \brief Image viewer - window from OpenCV
 * \author mstefanc
 */
class WindowOpenCV: public Base::Kernel
{
public:
	/*!
	 * Constructor.
	 */
	WindowOpenCV() {};

	bool setup() {
		cvNamedWindow(name().c_str(), 1);
		return true;
	}

	bool clean() {
		cvDestroyWindow(name().c_str());
		return true;
	}
};

/*
 * Register processor kernel.
 */
REGISTER_KERNEL("WindowOpenCV", Base::KERNEL_SINK, WindowOpenCV, Common::PanelEmpty)

#endif /* WINDOWOPENCV_HPP */
