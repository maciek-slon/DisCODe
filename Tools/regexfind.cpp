/*!
 * \file regexfind.cpp
 * \brief Small program searching in given directory for files matching
 * given regular expression.
 *
 * Uses boost::regex to handle regular expressions and boost::filesystem to
 * handle traversing through directories.
 *
 * \author mstefanc
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include <boost/regex.hpp>

using namespace std;


std::vector<std::string> searchFiles(const std::string & root, const std::string & regexp, bool with_path = true) {
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

int main(int argc, char * argv[]) {
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " root regex\n";
		return 0;
	}

	cout << "Searching in [" << argv[1] << "] with regex [" << argv[2] << "]\n";
	std::vector<std::string> ret = searchFiles(argv[1], argv[2]);

	for (size_t i = 0; i < ret.size(); ++i)
		std::cout << ret[i] << std::endl;

	return 0;
}
