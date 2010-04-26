/*!
 * \file Core.hpp
 * \brief Documentation and include file for Core namespace.
 * Also contains documentation mainpage.
 * \author mstefanc
 * \date 26.04.2010
 */

/*!
 * \namespace Core
 * \brief The Core namespace contains classes and structures essential for application's work, including threads synchronization,
 * tasks management and GUI.
 * \author tkornuta
 */



/*!
 * \mainpage FraDIA
 * The main goal of the FraDIA (Framework for Digital Image Analysis) was the merging of
 * image processing algorithms of all Robot Control and Recognition Systems Team's
 * diplomants in one tool, which will provide standardized and rapid method for the
 * implementation of diverse image processing algorithms.
 *
 * Current version wrappes the functionality of the OpenCV library
 * (http://sourceforge.net/projects/opencv/), offering the possibility of retrieving
 * images from a range of sources (camera, movie, image), as well as recording movies
 * (avi files) and catching single frames (png files). All images are stored in the
 * IplImage structure, derived from the Intel's IPP.
 *
 * User interface is constructed with the use of the FLTK (http://www.fltk.org/), which facilitates the UI
 * modifications required for every user's task.
 *
 * Framework automates the way of adding new tasks (interfaces and processing algorithms)
 * to the application - user has to implement two basic classes and enclose them
 * in dynamic library (so). At the start application retrieves information from all
 * shared objects found in current directory (.) and fills menu bar with names of
 * tasks, which enables switching between tasks without application's restart.
 *
 * FraDIA can work as a stand-alone application or as one of the processes of the controller based on the
 * MROCC++ framework. In the second case information retrieved from processed images is passed to the other
 * processes of the controller.
 *
 * Currently FraDIA also enables the use of GPGPU computations (basing on the CUDA library),
 * which can drastically reduce the time of images processing (e.g. filtering, color space transformations).
 *
 *
 * \author tkornuta
 * \date 10.08.2008
 */
