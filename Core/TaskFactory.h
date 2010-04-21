/*!
 * \file TaskFactory.h
 * \brief Class responsible for tasks management, switching, etc.
 * - class declaration
 * \author tkornuta
 * \date 10.03.2008
 */

#ifndef _TaskFactory_H_
#define _TaskFactory_H_

#include <pthread.h>
#include <vector>
#include <map>
#include <string>

#include "Panel.h"
#include "ImageProcessor.h"
#include "MainWindow.h"

using namespace std;
using namespace Base;

namespace Core
{

/*!
 * \typedef returnPanel
 * \brief Type representing function used for retrieving Panel object from SO.
 * \author tkornuta
 */
typedef Panel* (*returnPanelOLD)(void);

/*!
 * \typedef returnImageProcessor
 * \brief Type representing function used for retrieving Image Processor object from SO.
 * \author tkornuta
 */
typedef ImageProcessor* (*returnImageProcessorOLD)(void);

/*!
 * \class TaskFactory
 * \brief Class responsible for tasks management, switching, etc.
 * Implemented as a singleton.
 * \author tkornuta
 */
class TaskFactory
{
protected:
  /*!
   * Mutex used for task switching synchronization.
   */
  pthread_mutex_t *lock;

  /*!
   * Active panel.
   */
  Panel* active_panel;

  /*!
   * Active processor.
   */
  ImageProcessor* active_processor;

  /*!
   * Active task name.
   */
  string active_task_name;

  /*!
   * List of processors.
   */
  vector<ImageProcessor*> processors;

  /*!
   * List of tasks panels.
   */
  vector<Panel*> panels;

  /*!
   * List of task names.
   */
  vector<string> task_names;

  /*!
   * Private constructor - initializes mutex.
   */
  TaskFactory();


  /*!
   * Static menu call-back - switches task.
   */
  static void cb_SwitchTask(Fl_Widget* w_, void* data_);


  /*!
   * TaskFactory instance.
   */
  static TaskFactory* instance;

public:

  /*!
  * Method for accessing the TaskFactory instance.
  */
  static TaskFactory* getInstance();

  /*!
   * Lock the possibility of tasks switching.
   */
  void lockTaskSwitching() const;

  /*!
   * Unlock the possibility of tasks switching.
   */
  void unlockTaskSwitching() const;

  /*!
   * Method creates panel name from the SO name.
   */
  char* preparePanelName(string so_name_);

  /*!
   * Switches tasks, according to the desired task name.
   */
  void switchTaskByName(char* task_name_);

  /*!
   * Switches tasks, according to the desired task number.
   */
  void switchTaskByNumber(int task_no_);

  /*!
   * Synchronized active panel get.
   */
  Panel* getActivePanel();

  /*!
   * Synchronized active processor get.
   */
  ImageProcessor* getActiveImageProcessor();

  /*!
   * Method fills vector with names of found SO files.
   */
  void getSOList(string dir_, vector<string>& files_);

  /*!
   * Method loads all SO founded in the directory.
   */
  void loadSOs(MainWindow* window_);

  /*!
   * Method loads classes from given SO, creates
   * required objects - pairs of processors and panels.
   */
  void loadTaskClassesFromSO(MainWindow* window_, string filename_);

};

}

#endif

