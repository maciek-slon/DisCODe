/*!
 * \file ComponentManager.hpp
 * \brief File containing the ComponentManager class
 */

#ifndef COMPONENTMANAGER_HPP_
#define COMPONENTMANAGER_HPP_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "Component_Aux.hpp"
#include "DisCODeException.hpp"
#include "SharedLibraryCommon.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "ComponentFactory.hpp"

using namespace std;

namespace Core {

/*!
 * \class ComponentManager
 * \brief
 */
class ComponentManager
{
protected:
	/*!
	 * List of component factories properly loaded by the manager.
	 */
	typedef std::map <string, Core::ComponentFactory *> factories_map;

	std::map<std::string, factories_map> m_available_components;

	/*!
	 * List of created components
	 */
	std::map <string, Base::Component*> components;

	typedef std::pair<std::string, Base::Component *> comp_pair;

public:
	/*!
	 * Constructor
	 */
	ComponentManager()
	{
		LOG(LTRACE) << "ComponentManager: Hello private \n";
	}

	/*!
	 * Public destructor.
	 */
	~ComponentManager()
	{
		LOG(LTRACE) << "ComponentManager: Goodbye public\n";
	}

	void release() {
		BOOST_FOREACH(comp_pair comp, components) {
			LOG(LINFO) << "Removing component: " << comp.first << " (ptr=" << comp.second << ")";
			delete comp.second;
		}
	}

	/*!
	 * Method tries to create components from all shared libraries loaded from the . directory.
	 */
	void initializeComponentsList(const std::vector<std::string> dcl_locations)
	{
		int total_components = 0;

		if (dcl_locations.size() < 1) {
			LOG(LWARNING) << "No DCL locations speecified.";
			return;
		}

		std::string dcl_location;
		std::vector<std::string> dcls;
		BOOST_FOREACH(dcl_location, dcl_locations) {
			std::vector<std::string> tmp_dcls = Utils::getSubdirs(dcl_location, true);
			dcls.insert( dcls.end(), tmp_dcls.begin(), tmp_dcls.end() );
		}


		BOOST_FOREACH(dcl_location, dcls) {

			std::string dcl_name = boost::filesystem::path(dcl_location).filename().string();
			LOG(LINFO) << "Loading components from " << dcl_name;

			if (boost::filesystem::exists(dcl_location+"/dist/cache.txt")) {
				LOG(LINFO) << "Loading component list from cache file...";
				std::ifstream infile(dcl_location+"/dist/cache.txt");
				std::string line;
				std::vector<std::string> dcl;
				while (std::getline(infile, line))
				{
					boost::split(dcl, line, boost::is_any_of("\t"));
					// Create component empty "shell".
					ComponentFactory* k = new ComponentFactory(dcl_location + "/dist/lib/" + dcl[1]);
					m_available_components[dcl_name][dcl[0]] = k;
					++total_components;
					LOG(LINFO) << dcl[0] << " from " << dcl[1];
				}
			} else {

				// Get filenames.
				vector <string> files;
				try {
					getSOList(dcl_location + "/dist/lib", files);
				}
				catch(...) {
					LOG(LWARNING) << "Something bad happened when loading file list from " << dcl_location << ". Skipping.";
					continue;
				}

				// Check number of so's to import.
				if (files.empty()) {
					LOG(LWARNING) << "ComponentManager: There are no components in " << dcl_location;
					continue;
				}

				// Iterate through so names and add retrieved components to list.
				BOOST_FOREACH(string file, files)
				{
					// Create component empty "shell".
					ComponentFactory* k = new ComponentFactory(file);
					// Try to initialize component.
					if (k->lazyInitialize(file))
					{
						// Add component to list.
						m_available_components[dcl_name][k->getName()] = k;
						++total_components;
					} else {
						// Delete incorrect component.
						delete (k);
					}
				}//: FOREACH

			}//: IF

		}//: FOREACH

		// Check number of successfully loaded components.
		LOG(LNOTICE) << "Found " << total_components << " components in " << m_available_components.size() << " DCLs.";
	}

	/*!
	 *
	 */
	void deactivateComponentList() {
		// \todo: release all factories
	}

	/*!
	 *
	 * @param name
	 * @param type
	 * @return
	 */
	Base::Component* createComponent(const std::string & name, const std::string & dcl, const std::string & type) {
		if (components.count(name) > 0) {
			LOG(LWARNING) << "Module " << name << " already created. Returning previous one.\n";
			return components[name];
		}

		if (m_available_components.count(dcl) < 1) {
			LOG(LERROR) << "DCL " << dcl << " either not available or empty!\n";
			LOG(LNOTICE) << "When creating component: " << name << " [" << dcl << ":" << type << "]";
			throw Common::DisCODeException("Create component");
		}

		if (m_available_components[dcl].count(type) < 1) {
			LOG(LERROR) << "Component type " << type << " not found in " << dcl;
			LOG(LNOTICE) << "When creating component: " << name << " [" << dcl << ":" << type << "]";
			throw Common::DisCODeException("createComponent");
		}

		components[name] = m_available_components[dcl][type]->create(name);
		LOG(LDEBUG) << name << " [" << type << ":" << dcl << "] component created";
		return components[name];
	}

	/*!
	 *
	 * @param name
	 * @return
	 */
	Base::Component * getComponent(const std::string & name) {
		if (components.count(name) < 1) {
			LOG(LERROR) << "Module " << name << " can't be found!\n";
			throw Common::DisCODeException("getComponent");
		}

		return components[name];
	}

protected:

	/*!
	 *
	 * @param dir_
	 * @param files
	 */
	void getSOList(string dir_, vector <string>& files)
	{
		std::string regexp = "\\w*.";
		regexp += LIB_EXT;

		LOG(LTRACE) << "LIB_EXT = " LIB_EXT << "\n";

		files = Utils::searchFiles(dir_, regexp);
	}

};

}//: namespace Core

#endif /* COMPONENTMANAGER_HPP_ */
