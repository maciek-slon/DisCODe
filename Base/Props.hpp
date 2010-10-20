/*!
 * \file Props.hpp
 * \brief Base class for property structures.
 */

#ifndef PROPS_HPP_
#define PROPS_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/numeric/ublas/matrix.hpp>

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

protected:
	/*!
	 * Parse matrix from string.
	 * Throws exception if failed.
	 * @param rows
	 * @param cols
	 * @return
	 */
	boost::numeric::ublas::matrix<double> str2mat(const std::string& str, int rows, int cols) const;
};

}//: namespace Base

#endif /* PROPS_HPP_ */
