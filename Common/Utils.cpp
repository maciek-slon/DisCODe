/*!
 * \file Utils.cpp
 * \brief Utility functions - implementation
 * \author mstefanc
 * \date Jun 9, 2010
 */

#include "Utils.hpp"

#include <string>
#include <vector>

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include <boost/regex.hpp>

using namespace std;

namespace Utils {

std::vector<std::string> searchFiles(const std::string & root, const std::string & regexp, bool with_path) {
	boost::regex e(regexp);
	std::vector<std::string> ret;

	path dir_path(root);
	directory_iterator end_itr; // default construction yields past-the-end
	for ( directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
		std::string fname = itr->path().filename();
		if (regex_match(fname, e)) {
			if (with_path)
				ret.push_back(itr->path().file_string());
			else
				ret.push_back(fname);
		}
	}

	return ret;
}

} //: namespace Utils
