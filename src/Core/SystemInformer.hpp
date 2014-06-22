#ifndef SYSTEMINFORMER_HPP_
#define SYSTEMINFORMER_HPP_

#include "Informer.hpp"
#include "CommandInterpreter.hpp"

#include <boost/lexical_cast.hpp>

namespace Core {

class SystemInformer : public Informer {
public:
	SystemInformer(volatile bool & f, int loglvl=2) : m_flag(f), m_loglvl(loglvl) {

	}

	void registerHandlers(CommandInterpreter & ci) {
		ci.addHandler("kill", boost::bind(&SystemInformer::kill, this, _1));
		ci.addHandler("getLogLvl", boost::bind(&SystemInformer::getLogLvl, this, _1));
	}

	std::string kill(std::vector<std::string> /* args */) {
		m_flag = false;
		return "OK";
	}

	std::string getLogLvl(std::vector<std::string> /* args */) {
		return boost::lexical_cast<std::string>(m_loglvl);
	}

private:
	volatile bool & m_flag;
	int m_loglvl;
};

}

#endif /* SYSTEMINFORMER_HPP_ */
