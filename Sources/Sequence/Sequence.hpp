/*!
 * \file Sequence.hpp
 * \brief Class responsible for retrieving images from image sequences - class declaration.
 */


#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <vector>
#include <string>

#include <cv.h>

namespace Sources {
namespace Sequence {

struct Props : public Base::Props {

	std::string directory;
	std::string pattern;

	bool sort;
	bool prefetch;


	void load(const ptree & pt) {
		directory = pt.get("directory", ".");
		pattern = pt.get("pattern", ".*\\.jpg");

		sort = pt.get("sort", true);
		prefetch = pt.get("prefetch", false);
	}

	void save(ptree & pt) {
		pt.put("directory", directory);
		pt.put("pattern", pattern);
		pt.put("sort", sort);
		pt.put("prefetch", prefetch);
	}
};

/*!
 * \class Sequence
 * \brief Class responsible for retrieving images from image sequences.
 */
class Sequence : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	Sequence(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~Sequence();

	/*!
	 * Return sequence properties
	 */
	Base::Props * getProperties() {
		return &props;
	}

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
	Base::DataStreamOut<cv::Mat> out_img;

private:
	/**
	 * Fill list of files according to pattern
	 *
	 * \return true, if there is at least one file found, false otherwise
	 */
	bool findFiles();

	/// list of files in sequence
	std::vector<std::string> files;

	/// current frame
	cv::Mat img;

	/// index of current frame
	int frame;

	/// sequence properties
	Props props;
};

}//: namespace Sequence
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_SOURCE_COMPONENT("Sequence", Sources::Sequence::Sequence, Common::Panel_Empty)

#endif /* SEQUENCE_HPP_ */
