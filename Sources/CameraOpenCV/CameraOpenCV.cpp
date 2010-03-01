/*!
 * \file CameraOpenCV.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-02-28
 */

#ifndef CAMERAOPENCV_HPP_
#define CAMERAOPENCV_HPP_

#include <string>

#include "../../Base/Kernel_Aux.hpp"
#include "CameraOpenCV_Panel.hpp"

/*!
 * \class CameraOpenCV
 * \brief
 *
 * \author mstefanc
 */
class CameraOpenCV : public Base::Kernel {
public:
	CameraOpenCV(std::string n = "CameraOpenCV") : Kernel(n) {};

private:

};

REGISTER_KERNEL("CameraOpenCV", Base::KERNEL_SOURCE, CameraOpenCV, CameraOpenCV_Panel);

#endif /* CAMERAOPENCV_HPP_ */