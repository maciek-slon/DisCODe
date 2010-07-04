/*!
 * \file Source_Image.hpp
 * \brief Class responsible for retrieving images from files - class declaration.
 * \author tkornuta
 * \date 2009-11-05
 */


#ifndef IMAGE_SOURCE_HPP_
#define IMAGE_SOURCE_HPP_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Image_Panel.hpp"
#include "DataStream.hpp"

namespace Sources {
namespace Image {

/*!
 * \class Image_Source
 * \brief Class responsible for retrieving images from files.
 * \author tkornuta
 * \date 2009-11-05
 */
class Image_Source : public Base::Kernel {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	Image_Source();

	/*!
	 * Destructor.
	 */
	virtual ~Image_Source();

	/*!
	 * Connects source to given device.
	 */
	bool initialize();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool finish();

	/*!
	 * Retrieves data from device.
	 */
	int step();

protected:
	/// Event signaling that new image was retrieved.
	Base::Event * newImage;

	/// Output data stream
	Base::DataStreamOut<int> out_delay;
};

}//: namespace Image
}//: namespace Sources

/*
 * Register source kernel.
 */
REGISTER_SOURCE_KERNEL("Image", Sources::Image::Image_Source, Sources::Image::Image_Panel)

#endif /* IMAGE_SOURCE_HPP_ */
