/*!
 * \file Utils.hpp
 * \brief Utility functions - declaration
 * \author mstefanc
 * \date 2010-06-09
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <vector>

namespace Utils {


/*!
 * \brief Find all files matching given regular expression in specified location
 *
 * \param root directoory to search files
 * \param regexp regular expression to test file names
 * \param with_path if set to true, then output will be path+filename, otherwise only filename will be returned
 */
std::vector<std::string> searchFiles(const std::string & root, const std::string & regexp, bool with_path = true);

} //: namespace Utils

#endif /* UTILS_HPP_ */
