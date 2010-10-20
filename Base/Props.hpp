/*!
 * \file Props.hpp
 * \brief Base class for property structures.
 */

#ifndef PROPS_HPP_
#define PROPS_HPP_

#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;

namespace Base {


/*!
 * \class Props
 * \brief Base class for property structures.
 *
 * All structs that needs to be loaded and saved to config files should inherit
 * from Props and implement two methods - load and save. Those methods take ptree
 * as argument, so it's rather flexible - config can be kept in separate files for
 * every module or it can be subnode of bigger property tree.
 *
 * \par Defining own property structures
 *
 * Assuming that we need two properties - name of object and it's serial number
 * we can create following structure:
 *
 * \code
 * struct Properties {
 * 	// module name
 * 	std::string name;
 *
 * 	// module serial number
 *	int serial;
 * };
 * \endcode
 *
 * To allow this parameters to be read and written from config file we have to inherit
 * Base::Props and implement save and load methods.
 *
 * \code
 * struct Properties : public Base::Props {
 * 	// module name
 * 	std::string name;
 *
 * 	// module serial number
 * 	int serial;
 *
 * 	// load settings
 * 	void load(const ptree & pt) {
 * 		name = pt.get("props.name", "Default name");
 * 		serial = pt.get("props.serial", 0);
 * 	}
 *
 * 	// save settings
 * 	void save(ptree & pt) {
 * 		pt.put("props.name", name);
 * 		pt.put("props.serial", serial);
 * 	}
 * };
 * \endcode
 *
 */
class Props {

public:
	/*!
	 * Load settings
	 *
	 * @param pt root property tree to load settings from
	 */
	virtual void load(const ptree & pt) = 0;

	/*!
	 * Save settings
	 *
	 * @param pt root property tree to save settings
	 */
	virtual void save(ptree & pt) = 0;
//
//protected:
//	cv::Mat str2mat(cv::Size size, std::string s) {
//		std::stringstream ss;
//		cv::Mat mat = cv::Mat::eye(size, CV_32F);
//		double val;
//
//		ss << s;
//
//		for (int i = 0; i < size.height; ++i) {
//			for (int j = 0; j < size.width; ++j) {
//				ss >> val;
//				mat.at<float>(i,j) = val;
//			}
//		}
//
//		return mat;
//	}
};

}//: namespace Base

#endif /* PROPS_HPP_ */
