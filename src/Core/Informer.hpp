#ifndef INFORMER_HPP
#define INFORMER_HPP

namespace Core {

class CommandInterpreter;

class Informer {
public:
	virtual void registerHandlers(CommandInterpreter & ci) = 0;

	virtual ~Informer() {}
};

}

#endif // INFORMER_HPP
