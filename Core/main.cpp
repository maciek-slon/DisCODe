/*!
 * \file main.cpp
 * \brief Main body responsible for menu showing
 * and image processing.
 * \author tkornuta
 * \date 11.09.2007
 */

#include <cstring>
#include <iostream>

#include "FraDIAException.hpp"
#include "ConnectionManager.hpp"
#include "KernelManager.hpp"
#include "KernelFactory.hpp"
#include "Configurator.hpp"
#include "Executor.hpp"
#include "Logger.hpp"

using namespace std;
using namespace Common;
using namespace Core;

/*!
 * Main body - creates two threads - one for window and and one
 * for images acquisition/processing.
 */
int main(int argc, char* argv[])
{
	Configurator configurator;
	KernelManager kernelManager;

	try {
		// FraDIA config filename.
		std::string config_name;
		// Check whether other file wasn't pointed.
		if (argc == 2)
			config_name = argv[1];
		else
			// Default configuration file.
			config_name = "config.xml";

		kernelManager.initializeKernelsList();

		configurator.loadConfiguration(config_name);

		// Test code.

		Core::Executor ex1;

		Base::Kernel * src = kernelManager.createKernel("Source", "CameraUniCap");

		if (src->getProperties())
			src->getProperties()->load(ptree());
		src->initialize();

		Base::Kernel * proc = kernelManager.createKernel("Window", "OpenCVWnd");

		if (proc->getProperties())
			proc->getProperties()->load(ptree());
		proc->initialize();

		src->printEvents();
		src->printHandlers();
		src->printStreams();

		proc->printEvents();
		proc->printHandlers();
		proc->printStreams();

		// add components to separate threads
		ex1.addKernel(src, true);
		ex1.addKernel(proc);

		// connect src -> newImage event to proc -> onNewImage handler
		Base::EventHandlerInterface * h = proc->getHandler("onNewImage");
		//src->getEvent("newImage")->addHandler(ex2.scheduleHandler(h));
		src->getEvent("newImage")->addHandler(h);

		// connect src -> out_delay data stream to proc -> in_delay data stream
		Base::Connection * con_1 = CONNECTION_MANAGER.get("con_1");
		con_1->addListener(proc->getStream("in_img"));
		if (src->getStream("out_img")) {
			src->getStream("out_img")->setConnection(con_1);
		} else {
			cout << "Stream find error!\n";
		}

		// set parameters of each thread executor
		ex1.setExecutionMode(Executor::ExecPassive);
		ex1.setInterval(0.04);

		// start both threads
		ex1.start();

		Common::Thread::msleep(3000);

		// stop threads
		ex1.finish();

		// wait for both threads to finish execution
		ex1.wait(1000);

		// End of test code.

		src->finish();
		proc->finish();

		kernelManager.deactivateKernelList();

	}//: try
	catch (exception& ex){
		cout << "Fatal error:\n";
		// If required print exception description.
		if (!strcmp(ex.what(), ""))
			LOG(FATAL) << ex.what() << "\n";

		exit(EXIT_FAILURE);
	}//: catch
}
