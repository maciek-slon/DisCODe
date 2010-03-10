#include <iostream>
#include <boost/extension/extension.hpp>

// Any exported function or variable must be declared
// extern "C" to avoid C++ name mangling.
extern "C"
// Depending on the compiler and settings,
// it may be necessary to add a specific export
// declaration. The BOOST_EXTENSION_EXPORT_DECL
// adds this if necessary.
void BOOST_EXTENSION_EXPORT_DECL
boost_extension_hello_world(int repetitions) {
  for (int i = 0; i < repetitions; ++i) {
    std::cout << "Hello World 2" << std::endl;
  }
}
