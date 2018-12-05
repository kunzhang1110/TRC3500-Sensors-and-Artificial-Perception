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
#include <math.h>

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


	int thresh = 115;
	double m00=0,m01=0,m10=0,m11=0,m20=0,m02=0;
    // Convert half of the image to gray
    for( int y = 0; y < frame->height; y++) {
      for( int x = 0; x < frame->width; x++) {
	// This is a pointer to the start of the current row.
	//  Note: The image is stored as a 1-D array which is mapped back
	//  into 2-space by multiplying the widthStep (the image width rounded to
	//  a "nice" value, eg a multiple of 4 or 8 depending on the OS and CPU)
	//  by the row number.
		  uchar *row = (uchar*)(frame->imageData + frame->widthStep * y );

	int gray = ( row[ x*3 ] + row[ x*3+1 ] + row[ x*3+2 ] ) / 3;

/*	if (gray>max_row) {
		max_row = gray;
	}
	if (gray<min_row) {
		min_row = gray;
	}*/
	row[ x*3     ] = gray;
	row[ x*3 + 1 ] = gray;
	row[ x*3 + 2 ] = gray;
	double tau = 0;
	if(gray>thresh){
		tau = 0;
	}else{
		tau = 1;
	}

	m00 = m00+tau;
	m01 = m01+tau*x;
	m10 = m10+tau*y;
	m11 = m11+tau*x*y;
	m02 = m02+tau*x*x;
	m20 = m20+tau*y*y;
      }
    }

	// centre of area
	long i0 = (long)(m10/m00);
	long j0 = (long)(m01/m00);
	printf("centre of area, Y = %d, X = %d\n",i0,j0);

	double theta = 0.5*(atan((2*(m00*m11-m10*m01))/((m00*m20-m10*m10)-(m00*m02-m01*m01))));
	theta = theta*180/3.1415926575897;

	//thresh = (max_row + min_row) /2;
	//printf("min = %d\n",min_row);
	//printf("max = %d\n",max_row);
	//printf("thresh = %d\n",thresh);
	//printf("m00 = %f\n",m00);
	//printf("m20 = %f\n",m20);
	printf("Orientation = %f\n",theta);
	//Calculate moments
	//printf("height = %d\n",frame->height);
	//printf("width = %d\n",frame->width);


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

