/*!
 * \file TaskFactory.cc
 * \brief Class responsible for tasks management, switching, etc.
 * - methods declarations
 * \author tkornuta
 * \date 10.03.2008
 */

#include "TaskFactory.h"
#include "DataSynchronizer.h"
#include "XMLDataSynchronizer.h"
#include "FraDIAException.h"

#include "highgui.h"

#include <iostream>
#include <vector>
#include <string>

#include <regex.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include "FraDIAException.h"

using namespace std;
//using std::string;
using namespace Core;
using namespace Common;

namespace Core {

/*!
 * Set default TaskFactory instance to null.
 */
TaskFactory* TaskFactory::instance = NULL;

/*!
 * Private constructor - initializes mutex.
 */
TaskFactory::TaskFactory() {
	// Initialize mutex.
	lock = new pthread_mutex_t();
	pthread_mutex_init(lock, NULL);
	active_processor = NULL;
	active_panel = NULL;
}

/*!
 * Method for accessing the TaskFactory instance.
 */
TaskFactory* TaskFactory::getInstance() {
	// Create new instance if required.
	if (instance == NULL)
		instance = new TaskFactory();
	// Return instance.
	return instance;
}

/*!
 * Lock the possibility of tasks switching.
 */
void TaskFactory::lockTaskSwitching() const {
	pthread_mutex_lock(lock);

}

/*!
 * Unlock the possibility of tasks switching.
 */
void TaskFactory::unlockTaskSwitching() const {
	pthread_mutex_unlock(lock);
}

/*!
 * Synchronized active panel get.
 */
Panel* TaskFactory::getActivePanel() {
	return active_panel;
}

/*!
 * Synchronized active processor get.
 */
ImageProcessor* TaskFactory::getActiveImageProcessor() {
	return active_processor;
}

/*!
 * Static menu callback - switches task.
 */
void TaskFactory::cb_SwitchTask(Fl_Widget* w_, void* data_) {
	//Retrieve task name.
	char* name = (char*) data_;
	// Switch task.
	TaskFactory::getInstance()->switchTaskByName(name);
}

/*!
 * Method creates panel name from the SO name.
 */
char* TaskFactory::preparePanelName(string so_name_) {
	// Change all '_' to '/'.
	//	replace(so_name_.begin(), so_name_.end(), '_', '/');

	// Replaces all occurrences of old_ character with the new_ one.
	string new_string = string("/");
	// Find first space.
	int position = so_name_.find('_');
	// Iterate through string.
	while (position != so_name_.npos) {
		// Replace one char.
		so_name_.replace(position, 1, new_string);
		position = so_name_.find('_', position + 1);
	}//: while

	//string name = string("Task/") + so_name_;
	string name = so_name_;
	cout <<"FraDIA: Loading "<< name << " task.\n";
	// Allocate memory.
	char* panel_name = new char[80];
	strcpy(panel_name, name.c_str());
	// Return name.
	return panel_name;
}

/*!
 * Switches tasks, according to the desired task name.
 */
void TaskFactory::switchTaskByName(char* task_name_) {
	string taskname = string(task_name_);
	int index = -1;
	// Get index of new active panel.
	for (unsigned int i = 0; i < panels.size(); i++)
		if ((panels[i]->getPanelName()).compare(taskname) == 0)
			index = i;

	// Check founded number.
	if (index < 0)
		throw FraDIAException(string("FraDIA: Selected task \'") + task_name_ + string("\' doesn't exists."));

	// Switch task.
	switchTaskByNumber(index);

	// Remember selected task in XML config.
	XMLDataSynchronizer::getInstance()->setAttributeValue("/settings", "default_task", active_task_name.c_str());
}

/*!
 * Switches tasks, according to the desired task number.
 */
void TaskFactory::switchTaskByNumber(int task_no_) {
	// Get synchronizer instance.
	DataSynchronizer* ds = DataSynchronizer::getInstance();

	// Lock task switching.
	lockTaskSwitching();

	if (active_processor != NULL) {
		// Save current processor settings.
		active_processor->saveSettings();
		// Hide panel.
		active_panel->hide();
		// Destroy all openCV windows.
		ds->setDestroyWindows(true);
	}//: if !NULL

	// Set new active panel and processor.
	active_task_name = task_names[task_no_];
	active_processor = processors[task_no_];
	active_panel = panels[task_no_];
	active_panel->show();
	// Pass new settings to datasynchronizer.
	active_processor->loadSettings();

	// Unlock task switching.
	unlockTaskSwitching();
}

/*!
 * Method fills vector with names of found SO files.
 */
void TaskFactory::getSOList(string dir_, vector<string>& files_) {
	DIR *dp;
	struct dirent *dirp;
	// Try to open directory.
	if ((dp = opendir(dir_.c_str())) == NULL) {
		string e = string("FraDIA: Error while opening ") + dir_;
		throw FraDIAException(e.c_str());
	}//: if

	// Create regular expression.
	regex_t reg;
	regcomp(&reg, ".*[.]so", REG_NOSUB);

	// Iterate through files in directory.
	while ((dirp = readdir(dp)) != NULL)
		// If they fit to the regex - add them to list.
		if (regexec(&reg, dirp->d_name, 0, NULL, REG_NOTEOL) == 0)
			files_.push_back(dir_ + "/" + string(dirp->d_name));

	// Close directory.
	closedir(dp);
	// Free memory.
	regfree(&reg);
}

/*!
 * Method loads classes from given SO, creates
 * required objects and adds new option to menubar.
 */
void TaskFactory::loadTaskClassesFromSO(MainWindow* window_, string filename_) {
	try {
		char* err;
		// Prepare task_name.
		char* task_name = preparePanelName(filename_.substr(2, filename_.size() - 5));

		// Try to open dll.
		void *obj = dlopen(filename_.c_str(), RTLD_LOCAL | RTLD_LAZY);
		// Validate operation.
		if ((err = dlerror()) != NULL)
			throw FraDIAException(err);

		// Variable used for storing address of the function returning created image processor.
		returnImageProcessorOLD rip;
		// Try to retrieve method.
		rip = (returnImageProcessorOLD) dlsym(obj, "returnImageProcessor");
		// Validate operation.
		if ((err = dlerror()) != NULL)
			throw FraDIAException(err);

		// Get image processor.
		ImageProcessor* ip = rip();

		// Variable used for storing address of the function returning created task panel.
		returnPanelOLD rup;
		// Try to retrieve method.
		rup = (returnPanelOLD) dlsym(obj, "returnPanel");
		// Validate operation.
		if ((err = dlerror()) != NULL)
			throw FraDIAException(err);

		// Get task panel.
		Panel* up = rup();
		// Initialize panel.
		up->createContent();
		up->end();
		up->hide();
		up->setPanelName(task_name);

		// Check whether main window exists or not.
		if (window_ != NULL) {
			// Add new option to menubar.
			window_->menubar->add(task_name, 0, cb_SwitchTask, (void*) task_name);//, FL_CTRL+string(i_));//
		}//: if !window

		// Add variables to lists.
		panels.push_back(up);
		processors.push_back(ip);
		task_names.push_back(filename_.substr(2, filename_.size() - 5));

		// Close so.
		//  dlclose(obj);
	} catch (FraDIAException& ex) {
		cout << "Error: " << ex.what() << endl;
	}
}

/*!
 * Method loads all SO founded in the directory.
 */
void TaskFactory::loadSOs(MainWindow* window_) {
	// Vector for storing SO filenames.
	vector<string> files = vector<string> ();
	// Get filenames.
	getSOList(".", files);

	// Check number of so's to import.
	if (files.size() == 0)
		throw FraDIAException("FraDIA: There must be at least one dynamic library containing task (so).");

	// Iterate through so names and add retrieved objects to lists.
	for (unsigned int i = 0; i < files.size(); i++)
		loadTaskClassesFromSO(window_, files[i]);

	// Check number of succesfully imported so's.
	if (panels.size() == 0)
		throw FraDIAException("FraDIA: There are no compatible task dynamic libraries (so).");

	// Set task 0 as default.
	switchTaskByNumber(0);

}

}//: namespace
