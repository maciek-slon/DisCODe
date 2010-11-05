/*!
 * \file xmltree.cpp
 * \brief Simple program printing XML-tree from given file.
 *
 * File name must be passed as first argument, tree is printed in very simple
 * form, with indents indicating child-parent relationship. For each node
 * value is printed, event if there is no value associated (in that case empty
 * string is shown).
 *
 * \author mstefanc
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <exception>
#include <iostream>

using boost::property_tree::ptree;

typedef std::pair<std::string, ptree> sp;

void colprint(const std::string & key, const std::string & val, int colwidth, int indent = 0, char ch = '.') {
	for (int i = 0; i < indent; ++i)
		std::cout << " ";

	std::cout << key << " ";

	for (int i = key.length() + indent; i < colwidth-2; ++i)
		std::cout << ch;

	std::cout << " \"" << val << "\"";

	std::cout << std::endl;
}

void printTree(const ptree & pt, int lvl) {
	BOOST_FOREACH( sp p, pt) {
		colprint(p.first, pt.get(p.first, ""), 25, lvl*2);
		printTree(p.second, lvl+1);
	}
}

void iterate(const std::string &filename) {
	// Create empty property tree object
	using boost::property_tree::ptree;
	ptree pt;

	// Load XML file and put its contents in property tree.
	// No namespace qualification is needed, because of Koenig
	// lookup on the second argument. If reading fails, exception
	// is thrown.
	read_xml(filename, pt);

	printTree(pt, 0);
}

int main(int argc, const char * argv[])
{
	if (argc < 2) {
		std::cout << "Supply filename.\n";
		return 1;
	}

    try
    {
    	iterate(argv[1]);
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
