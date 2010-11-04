/*!
 * \file Subtask.cpp
 * \brief
 * \date 08-07-2010
 */

#include "Subtask.hpp"
#include "Component.hpp"
#include "Logger.hpp"

namespace Core {


Subtask::~Subtask()
{

}

Subtask & Subtask::operator +=(Base::Component *comp)
{
	components.push_back(comp);
	return *this;
}

bool Subtask::stop()
{
	bool ret = true;
	for (comp_it it = components.begin(); it != components.end(); ++it) {
		if (! (*it)->stop()) ret = false;
	}
	return ret;
}

bool Subtask::start()
{
	bool ret = true;
	for (comp_it it = components.begin(); it != components.end(); ++it) {
		if (! (*it)->start()) ret = false;
	}
	return ret;
}

bool Subtask::finish()
{
	bool ret = true;
	for (comp_it it = components.begin(); it != components.end(); ++it) {
		LOG(LTRACE) << "Finishing " << (*it)->name();
		if (! (*it)->finish()) ret = false;
	}
	return ret;
}

}
