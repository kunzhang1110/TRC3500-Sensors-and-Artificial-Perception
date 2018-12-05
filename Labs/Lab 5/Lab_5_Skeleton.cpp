////////////////////////////////////////////////////////////////
// Skeleton program for TRC3500
// Grabs images from a USB camera using OpenCV
// Written by Andy Russell 09th February 2006
// Modified by Michael Curtis 2011-2012 - updated for newer OpenCV
/////////////////////////////////////////////////////////////////
#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <stdio.h>

using namespace std;

////////////////////////////////////////////////////////////////
// main - initialises OpenCV and captures an image and changes it
////////////////////////////////////////////////////////////////
int main( )
{
  CvCapture* capture = 0;
  IplImage* processedImage = 0;

  cout<<"Andy's USB camera program"<<endl<<"Press 'q' to quit"<<endl;
 
  // Initializes capturing video from camera
  capture = cvCaptureFromCAM( -1 );
  if( !capture ) {
    fprintf(stderr,"Could not initialize capturing...\n");
    return -1;
  }


  // Creates window
  cvNamedWindow( "Camera image", 1 );

  // Camera image
  IplImage* frame = 0;

  // Grabs and returns a frame from camera
  frame = cvQueryFrame( capture );
  
  // Print details of image
  cout<<"image width ="<<frame->width<<" height ="<<frame->height;
  cout<<" depth ="<<frame->depth<<" channels ="<<frame->nChannels<<endl;

  do {
    // Grabs and returns a frame from camera
    frame = cvQueryFrame( capture );
    if( !frame ) {
      break;
    }

    // Convert half of the image to gray
    for( int y = 0; y < frame->height / 2; y++) {
      for( int x = 0; x < frame->width; x++) {
	// This is a pointer to the start of the current row.
	//  Note: The image is stored as a 1-D array which is mapped back
	//  into 2-space by multiplying the widthStep (the image width rounded to
	//  a "nice" value, eg a multiple of 4 or 8 depending on the OS and CPU)
	//  by the row number.
		  uchar *row = (uchar*)(frame->imageData + frame->widthStep * y );

	int gray = ( row[ x*3 ] + row[ x*3+1 ] + row[ x*3+2 ] ) / 3;

	row[ x*3     ] = gray;
	row[ x*3 + 1 ] = gray;
	row[ x*3 + 2 ] = gray;
      }
    }


    // Shows the resulting image in the window
    cvShowImage( "Camera image", frame );

  } while ('q'!=cvWaitKey(10));

  //tidy up

  // Releases the CvCapture structure
  cvReleaseCapture( &capture );
  // Destroys all the HighGUI windows
  cvDestroyAllWindows( );

  return 0;

} //end of main 
