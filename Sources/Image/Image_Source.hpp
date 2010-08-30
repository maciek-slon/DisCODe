/*!
 * \file Image_Source.hpp
 * \brief Class responsible for retrieving images from files - class declaration.
 * \author tkornuta
 * \date 2009-11-05
 */


#ifndef IMAGE_SOURCE_HPP_
#define IMAGE_SOURCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
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
class Image_Source : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	Image_Source(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~Image_Source();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

	/// Event signaling that new image was retrieved.
	Base::Event * newImage;

	/// Output data stream
	Base::DataStreamOut<int> out_delay;
};

}//: namespace Image
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_SOURCE_COMPONENT("Image", Sources::Image::Image_Source, Sources::Image::Image_Panel)

#endif /* IMAGE_SOURCE_HPP_ */
