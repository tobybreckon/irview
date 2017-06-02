// irview : simple viewer for adding false colour to IR or thermal images
//          / video / camera feed
// usage: prog {image/video file}

// Author : Toby Breckon, toby.breckon@durham.ac.uk

// Copyright (c) 2008 School of Engineering, Cranfield University
// Copyright (c) 2017 School of Engineering and Computing Sciences, Durham University
// License : GPL - http://www.gnu.org/licenses/gpl.html

/******************************************************************************/

#define BUILD_FOR_OPENCV_3 1

#if (BUILD_FOR_OPENCV_3)

#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>		// standard C++ I/O
#include <string>		// standard C++ I/O
#include <algorithm>    // includes max()

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std;

#else

#include "cv.h"       // open cv general include file
#include "highgui.h"  // open cv GUI include file

#include <stdio.h>
#include <ctype.h>

#endif

/******************************************************************************/
// setup the camera index properly based on OS platform

// 0 in linux gives first camera for v4l
//-1 in windows gives first device or user dialog selection

#ifdef linux
	#define CAMERA_INDEX 0
#else
	#define CAMERA_INDEX -1
#endif
/******************************************************************************/

#define PROG_ID_STRING "IrView v0.2- (c) Toby Breckon, 2008-2017+"
#define LICENSE_STRING "GPL - http://www.gnu.org/licenses/gpl.html"

static void print_help(char **name){
	printf("\n%s\n", PROG_ID_STRING);
	printf ("\t with OpenCV version %s (%d.%d.%d)\n",
					CV_VERSION,
					CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
	printf("%s\n\n", LICENSE_STRING);


	printf("Usage :%s [image/video file]\n", name[0]);
	printf("Camera interface: run with no file agrument for direct camera use\n");
	printf("\nKeyboard commands\n");
	printf("\t a - automatic scaling (default: on)\n");
	printf("\t b - show both false colour and original (default: off)\n");
	printf("\t c - toggle false colour (default: on)\n");
	printf("\t x - exit\n\n");
}

/******************************************************************************/

// concatenate 2 OpenCV Mat Objects side-by-side (in general)

Mat concatImages(Mat img1, Mat img2)
{
		Mat out = Mat(img1.rows, img1.cols + img2.cols, img1.type());
		Mat roi = out(Rect(0, 0, img1.cols, img1.rows));
		Mat roi2 = out(Rect(img1.cols, 0, img2.cols, img2.rows));

		img1.copyTo(roi);

		// depth of img1 is master, depth of img2 is slave
		// so convert if needed

		if (img1.depth() != img2.depth())
		{
				// e.g. if img2 is 8-bit and img1 32-bit - scale to range 0->1 (32-bit)
				// otherwise img2 is 32-bit and img1 is 8-bit - scale to 0->255 (8-bit)

				img2.convertTo(roi2, img1.depth(), (img2.depth() < img1.depth()) ? 1.0 / 255.0 : 255);
		} else {
				img2.copyTo(roi2);
		}
		return out;
}

/******************************************************************************/

int main( int argc, char** argv )
{

	IplImage* img = NULL;      // image object
	CvCapture* capture = NULL; // capture object

	const char* windowNameHSV = PROG_ID_STRING; // window name

 	IplImage* HSV = NULL;					// HSV image
 	IplImage* singleChannelH = NULL;		// Hue plain (from input image)
 	IplImage* singleChannelPlain = NULL; 	// constant plane for S & V


	bool keepProcessing = true;		// loop control flag
	char key = '\0';				// user input
	int  EVENT_LOOP_DELAY = 40;     // 40ms equates to 1000ms/25fps =
									// 40ms per frame

	bool useFalseColour = true;     // process flag - false colour
	bool useNormalisation = true;   // process flag - normalisation
	bool useConcatImage = false; 		// process flag - show concatenated images

  // if command line arguments are provided try to read image/video_name
  // otherwise default to capture from attached H/W camera

    if(
	  ( argc == 2 && (img = cvLoadImage( argv[1], 1)) != 0 ) ||
	  ( argc == 2 && (capture = cvCreateFileCapture( argv[1] )) != 0 ) ||
	  ( argc != 2 && (capture = cvCreateCameraCapture( CAMERA_INDEX )) != 0 )
	  )
    {
      // print help

	  print_help(argv);

	  // create window object (use flag=0 to allow resize, 1 to auto fix size)

	  cvNamedWindow(windowNameHSV, 0);
	  cvResizeWindow(windowNameHSV, 640, 480);

	  // if capture object in use (i.e. video/camera)
	  // get initial image from capture object

	  if (capture) {

		  // cvQueryFrame s just a combination of cvGrabFrame
		  // and cvRetrieveFrame in one call.

		  img = cvQueryFrame(capture);
		  if(!img){
			if (argc == 2){
				printf("End of video file reached\n");
			} else {
				printf("ERROR: cannot get next frame from camera\n");
			}
			exit(0);
		  }

	  }

	  // setup output image in HSV

	  HSV = cvCloneImage(img);
	  singleChannelH =
	  			cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	  singleChannelH->origin = img->origin;
	  IplImage* singleChannelV =
	  			cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	  singleChannelV->origin = img->origin;

	  // set single channel up for Saturation / Variance

	  singleChannelPlain =
	  			cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	  singleChannelPlain->origin = img->origin;
	  cvSet(singleChannelPlain, cvScalar(255));

	  // start main loop

	  while (keepProcessing) {

		  // if capture object in use (i.e. video/camera)
		  // get image from capture object

		  if (capture) {

			  // cvQueryFrame is just a combination of cvGrabFrame
			  // and cvRetrieveFrame in one call.

			  img = cvQueryFrame(capture);

			  // cvQueryFrame s just a combination of cvGrabFrame
			  // and cvRetrieveFrame in one call.

			  if(!img){
					if (argc == 2){
						printf("End of video file reached\n");
					} else {
						printf("ERROR: cannot get next frame from camera\n");
					}
					exit(0);
			  }

		  }

		  // extract first (or only input image channel)

		  if (img->nChannels > 1) {
		  		cvSetImageCOI(img, 1); // select channel 1, 0 means all channels
		  }

		  // we will use this for the Hue and Variance channels

		  cvCopy(img, singleChannelH);
		  cvCopy(img, singleChannelV);
		  cvSetImageCOI(img, 0);

		  // do colour normalisation (makes it look more impressive)

		  if (useNormalisation){
		  	cvNormalize(singleChannelH, singleChannelH, 0, 255,
			  												CV_MINMAX, NULL);
				cvNormalize(singleChannelV, singleChannelV, 0, 255,
			  												CV_MINMAX, NULL);
		  }

		  // do scaling to avoid Hue space wrap around (i.e. dark == bright!)
		  // N.B. changing the scaling factor and addition will vary the colour
		  // effect - OpenCV 8-bit Hue in range 0->120 => 0.5 * Hue + 90 maps
		  // all values to (wrap-around) 180->60 range in Hue.

		  cvConvertScale(singleChannelH, singleChannelH, 0.5, 90);

      // put it all back together in RGB

		  cvMerge(singleChannelH, singleChannelPlain,
		  										singleChannelV, NULL, HSV);
		  cvCvtColor(HSV, HSV, CV_HSV2BGR);

		  // display image in window

			if (useConcatImage){
				imshow(windowNameHSV, concatImages(cv::cvarrToMat(img), cv::cvarrToMat(HSV)));
			} else {
				if (useFalseColour){
			  	cvShowImage(windowNameHSV, HSV);
				} else {
			    cvShowImage(windowNameHSV, img);
			  }
			}

		  // start event processing loop

		  key = cvWaitKey(EVENT_LOOP_DELAY);

		  // process any keyboard input

		  switch (tolower(key))
			{
				case  'x':

					// if user presses "x" then exit

	   				keepProcessing = false;
					;
					break;
				case  'a':

					// toggle automatic scaling

					useNormalisation = (!useNormalisation);

					;
					break;
			 	case  'b':

						// toggle concatenated images

						useConcatImage = (!useConcatImage);

						;
						break;
				case  'c':

					// toggle false colour

					useFalseColour = (!useFalseColour);

					;
					break;
			}

	  }

    // destroy window objects

	  cvDestroyAllWindows();

    // destroy image object (if it does not originate from a capture object)

    if (!capture){
		  cvReleaseImage( &img );
    }
	  cvReleaseImage( &HSV );
	  cvReleaseImage( &singleChannelH );
	  cvReleaseImage( &singleChannelPlain );

      // all OK : main returns 0

      return 0;
    }

    // not OK : main returns -1

	print_help(argv);
    return -1;
}
/******************************************************************************/
