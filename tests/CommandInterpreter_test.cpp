#include "../src/Core/CommandInterpreter.hpp"

#include <vector>
#include <string>
#include <iostream>

std::string print(std::vector<std::string> args) {
	for (int i = 0; i < args.size(); ++i) {
		std::cout << args[i] << std::endl;
	}

	return "OK";
}

int main() {
	Core::CommandInterpreter interpreter;

	interpreter.addHandler("print", print);
	interpreter.execute("print:ela:to:zdzira");
	interpreter.execute("print3:ela:to:zdzira");

	return 0;
}
