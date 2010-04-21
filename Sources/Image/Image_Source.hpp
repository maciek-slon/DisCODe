/*!
 * \file Source_Image.hpp
 * \brief Class responsible for retrieving images from files - class declaration.
 * \author tkornuta
 * \date 2009-11-05
 */


#ifndef IMAGE_SOURCE_HPP_
#define IMAGE_SOURCE_HPP_

#include "Kernel_Aux.hpp"
#include "DataSource.hpp"
#include "Image_Panel.hpp"
#include "StringState.hpp"

namespace Sources {
namespace Image {

/*!
 * \class Image_Source
 * \brief Class responsible for retrieving images from files.
 * \author tkornuta
 * \date 2009-11-05
 */
class Image_Source : public Base::DataSource {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	Image_Source();

	/*!
	 * Destructor.
	 */
	~Image_Source();

	/*!
	 * Connects source to given device.
	 */
	void connect();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	void disconnect();

	/*!
	 * Retrieves data from device.
	 */
	void* retrieveData();


};

}//: namespace Image
}//: namespace Sources

/*
 * Register source kernel.
 */
REGISTER_SOURCE_KERNEL("Image", Sources::Image::Image_Source, Sources::Image::Image_Panel, Common::StringState::instance())

#endif /* IMAGE_SOURCE_HPP_ */
