#ifndef SYSTEMINFORMER_HPP_
#define SYSTEMINFORMER_HPP_

#include "Informer.hpp"
#include "CommandInterpreter.hpp"

namespace Core {

class SystemInformer : public Informer {
public:
	SystemInformer(volatile bool & f) : m_flag(f) {

	}

	void registerHandlers(CommandInterpreter & ci) {
		ci.addHandler("kill", boost::bind(&SystemInformer::kill, this, _1));
	}

	std::string kill(std::vector<std::string> args) {
		m_flag = false;
		return "OK";
	}

private:
	volatile bool & m_flag;
};

}

#endif /* SYSTEMINFORMER_HPP_ */
