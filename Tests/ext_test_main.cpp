#include <iostream>
#include <boost/extension/shared_library.hpp>
#include <boost/function.hpp>

int main() {
  using namespace boost::extensions;

  // In the Jamfile, shared libraries are set to have the same
  // prefix and extension, even on different operating systems.
  // This is for convenience in writing cross-platform code, but
  // is not required. All shared libraries are set to start with
  // "lib" and end with "extension".
  std::string library_path = "libtest.dll";

  // Create shared_library object with the relative or absolute
  // path to the shared library.
  shared_library lib(library_path);

  // Attempt to open the shared library.
  if (!lib.open()) {
    std::cerr << "Library failed to open: " << library_path << std::endl;
    return 1;
  }

  // Retrieve a function from the library, and store it in a Boost.Function
  // object. It is also possible to use function pointers, but the syntax
  // for Boost.Function is easier to understand. This retrieves a function
  // called "boost_extension_hello_world" with a void return type and a single
  // parameter of type int.
  boost::function<void (int)>
    f(lib.get<void, int>("boost_extension_hello_world"));

  // Check that the function was found.
  if (!f) {
    std::cerr << "Function not found!" << std::endl;
    return 1;
  }

  // Call the function from the shared library with
  // an integer parameter.
  f(4);
}
