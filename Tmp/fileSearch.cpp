#include <iostream>
#include <string>

#include "boost/filesystem.hpp"
using namespace boost::filesystem;

#include "boost/regex.hpp"

using namespace std;


void searchFiles(const std::string & root, const std::string & regexp) {
	boost::regex e(regexp);

	path dir_path(root);
	directory_iterator end_itr; // default construction yields past-the-end
	for ( directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
		std::string fname = itr->path().filename();
		//cout << fname << " ";
		if (regex_match(fname, e)) {
			cout << "Found: " << itr->path().file_string() << endl;
		}// else
		//	cout << " -\n";
	}
}

int main(int argc, char * argv[]) {
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " root regex\n";
		return 0;
	}

	cout << "Searching in [" << argv[1] << "] with regex [" << argv[2] << "]\n";
	searchFiles(argv[1], argv[2]);

	return 0;
}
