#include <iostream>

#include "../Core/ConnectionManager.hpp"
#include "Kernel_Aux.hpp"

#include "boost/filesystem.hpp"
using namespace boost::filesystem;

#include <boost/extension/shared_library.hpp>
using namespace boost::extensions;

#include <boost/function.hpp>

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(MSC_VER)
#  define LIB_EXT ".dll"
#else
#  define LIB_EXT ".so"
#endif

using namespace std;

void processLibrary(const std::string & fname) {
	shared_library lib(fname);
	if (!lib.open()) {
	    std::cerr << "Library failed to open: " << fname << std::endl;
	    return;
	}

	std::cout << "File name: " << fname << std::endl;

	//boost::function<std::string ()> f(lib.get<std::string>("returnName"));
	Base::returnName nam = lib.get<std::string>("returnName");
	Base::returnType typ = lib.get<Base::kernelType>("returnType");
	if ( !nam || !typ ) {
		std::cout << "\tInvalid Kernel library!\n";
	} else {
		std::cout << "\tKernel name: " << nam() << std::endl;
		switch(typ()) {
		case Base::KERNEL_PROCESSOR:
			std::cout << "\tType: PROCESSOR\n";
			break;
		case Base::KERNEL_SOURCE:
			std::cout << "\tType: SOURCE\n";
			break;
		case Base::KERNEL_SINK:
			std::cout << "\tType: SINK\n";
			break;
		default:
			std::cout << "\tType: UNKNOWN\n";
		}
	}

	std::cout << "\n";
}

int main()
{
	// find all libraries in current directory
	path dir_path(".");
	directory_iterator end_itr; // default construction yields past-the-end
	for ( directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
		if ( itr->path().extension() == LIB_EXT )
			processLibrary(itr->leaf());
	}

    return 0;
}

