/*!
 * \file V4L.h
 * \author szymek
 * \date 05.03.2009
 */
#include <stdio.h>
#include <stdlib.h>
#include "cv.h"
#include "highgui.h"
#include <vector>

#include <linux/types.h>
#include <linux/videodev2.h>
#include "PrintV4L2.h"
#include "VL.h"


#define CLEAR(x) memset (&(x), 0, sizeof (x))
/*!
 * io_method
 */


/*!
 * struct buffer
 */
struct buffer {
  void * start;
  size_t length;
};

/*!
 * \class V4L2
 * \author szymek
 */
class V4L2 : public VL
{

 private:
	/*!
	   * Window width.
	*/
  int width;
	/*!
	   * Window height.
	*/
  int height;
	/*!
	 * 	Palette
	 */
  int palette;
	/*!
	 * 	Interlacing
	 */

  v4l2_field interlace;
	/*!
	   * Device name.
	*/
  vector<string> V4L2_palettes;

  char device[100];
	/*!
	   * IO method.
	*/
  io_method io;
	/*!
	   * File descriptor.
	*/
  int fd;
	/*!
	   * Pointer to buffer with data.
	*/
  buffer *buffers;
	/*!
	   * Number of buffers.
	*/
  unsigned int n_buffers;

  /*!
   * Method reads single frame.
   */
  void *readFrame();
  /*!
   * Method initializes frame buffer when we choose DMA io_method.
   */
  void init_read(unsigned int buffer_size);
  /*!
    * Method initializes frame buffer when we choose MM io_method.
    */
  void init_mmap(void);
  /*!
    * Method initializes frame buffer when we choose US io_method.
    */
  void init_userp(unsigned int buffer_size);
  /*!
    * Single frame.
    */
  IplImage img;


 public:
	 /*!
	   * Method prints frame grabber settings.
	   */
  void printSettings();
  /*!
    * Method gets file descriptor.
    */
  int getFileDescriptor();
  /*!
    * (Simple) Method gets standard capabilities.
    */
  int get(struct v4l2_capability *cap);
  /*!
    * (Simple) Method gets video format.
    */
  int get(struct v4l2_format *fmt);
  /*!
    * (Simple) Method gets query variables.
    */
  int get(struct v4l2_queryctrl *ctrl);
  /*!
    * (Simple) Method gets control variables.
    */
  int get(struct v4l2_control *control);

  /*!
    * (Simple) Method sets video format.
    */
  int set(struct v4l2_format  fmt);
  /*!
    * (Simple) Method sets control variables.
    */
  int set(struct v4l2_control  control);

  /*!
    * Method initializes buffers.
    */
  void initBuffers();
  /*!
    * Method starts capture.
    */
  void startCapture();
  /*!
    * Method gets one frame.
    */
  void *getFrame();
  /*!
    * Method stops capture.
    */
  void stopCapture();
  /*!
    * Method deallocate buffer memory.
    */
  void freeBuffers();
  /*!
    * Method checks device standard.
    */
  int tryLibV4L2(io_method);
  /*!
    * Method gets information about device channels.
    */
  vector<string> getChannels();

  int imageBits(int palette);
/*!
 * Constructor
 */
  V4L2();
  /*
   *
   */
  V4L2(string param, string value);
 /*!
  * Destructor
  */
  ~V4L2();

  /*!
   * Method loads standard settings like brightness, contrast etc (without source_settings).
  */
bool loadFrameGrabber(int );
  /*!
   * Method gets one frame from frame grabber.
   */
IplImage * getOneFrame();
  /*!
	* Method  stops frame grabber and deallocate memory.
	*/
bool releaseFrameGrabber();

  /*!
	* Method  gets video property value like video standard or channel.
	*/
int  getVideoProperty(int property);
  /*!
	* Method  gets window property value like window width or height.
	*/
int  getWinProperty(int property);
  /*!
	* Method  gets picture property value like brightness or contrast.
	*/
int  getPicProperty(int property);

  /*!
	* Method  sets video property value like video standard or channel.
	*/
bool setVideoProperty(int channel, int norm );
 /*!
	* Method  sets window property value like window width or height.
	*/
bool setWinProperty(int property, int value);
  /*!
	* Method  sets picture property value like brightness or contrast.
	*/
bool setPicProperty(int property, int value);

void tryPalettes();

bool findPalette(string palette);

vector<string> getPalettes();
	/*
	 * Method return all information about frame grabber
	 */
string getFrameGrabberOptions();

vector<string> getIOMethod();

};
