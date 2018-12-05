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

int scanrow[640] = {0}, 
	EDGE_LINE_GROUP[60] = {0}, 
	code_left_1[4] = {0},
	code_left_2[4] = {0},
	code_left_3[4] = {0},
	code_left_4[4] = {0},
	code_left_5[4] = {0},
	code_left_6[4] = {0},
	code_right_1[4] = {0},
	code_right_2[4] = {0},
	code_right_3[4] = {0},
	code_right_4[4] = {0},
	code_right_5[4] = {0},
	code_right_6[4] = {0},
	LEFT_NUMBER[6] = {0},
	RIGHT_NUMBER[6] = {0};

//array initilization

char parity_left[6],parity_right[6];

int digit13(char par1, char par2,char par3,char par4,char par5,char par6);
void COMPARE_TABLE(int T1_width, int T2_width, int T4_width ,int *character, char *parity);
void RightDigit(int number,int LINE_1_R,int LINE_1_F,int LINE_2_R,int LINE_2_F,int endP);
void LeftDigit(int number,int LINE_1_R,int LINE_1_F,int LINE_2_R,int LINE_2_F,int beginP);
int T_Width(float Ti, float T);
////////////////////////////////////////////////////////////////
// main - initialises OpenCV and captures an image and changes it
////////////////////////////////////////////////////////////////
int main( )
{
  CvCapture* capture = 0;
  IplImage* processedImage = 0;

  cout<<"Andy's USB camera program"<<endl<<"Press any key to capture image for scan"<<endl;
  cout<<"Press and hold 'q' to quit"<<endl;
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

  int threshold=130;
  int black=0;
  int white=255;

  do {
	 cvWaitKey(100);
    // Grabs and returns a frame from camera
    frame = cvQueryFrame( capture );
    if( !frame ) {
      break;
    }

	int x;
	int y;

    // Convert half of the image to gray
    for( y = 0; y < frame->height; y++) {
      for( x = 0; x < frame->width; x++) {
	// This is a pointer to the start of the current row.
	//  Note: The image is stored as a 1-D array which is mapped back
	//  into 2-space by multiplying the widthStep (the image width rounded to
	//  a "nice" value, eg a multiple of 4 or 8 depending on the OS and CPU)
	//  by the row number.
		uchar *row = (uchar*)(frame->imageData + frame->widthStep * y );

		int gray = ( row[ x*3 ] + row[ x*3+1 ] + row[ x*3+2 ] ) / 3;

		if (y == (frame->height/2)){ //apply thresholding to image
				if (gray <= threshold )
				{
					scanrow[x]=1;
					//printf(",%d",x);
					gray = 0;


				}else{
					//printf("%d",0);
					scanrow[x]=0;
					gray = 225;
				}
		}
		row[ x*3     ] = gray;
		row[ x*3 + 1 ] = gray;
		row[ x*3 + 2 ] = gray;
		}
	}
	//printf("finish\n");
	int i = 0;
	int falledge = 0;
	for(i = 0 ;i<640 ; i++)
	{
		if (((scanrow[i]==0)&&(scanrow[i-1]==1))|((scanrow[i]==1)&&(scanrow[i-1]==0))){
			falledge+=1;
			EDGE_LINE_GROUP[falledge] = i;
			printf(",%d",i);
		}
	}
	printf("finish\n");
	int a = 5;
	/////////////////////////////////////////////////////left//////////////////////////////////////////
	for(i=0; i<4; i++){
		code_left_1[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	printf(",%d",code_left_1[0]);

	for(i=0; i<4; i++){
		code_left_2[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	printf(",%d",code_left_2[0]);
	//printf(",%d",a);

	for(i=0; i<4; i++){
		code_left_3[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	//printf(",%d",a);

	for(i=0; i<4; i++){
		code_left_4[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	//printf(",%d",a);

	for(i=0; i<4; i++){
		code_left_5[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	for(i=0; i<4; i++){
		code_left_6[i]=EDGE_LINE_GROUP [a];
		a++;
	}




	a = 33;
	/////////////////////////////////////////////////////right//////////////////////////////////////////
	for(i=0; i<4; i++){
		code_right_1[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	for(i=0; i<4; i++){
		code_right_2[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	for(i=0; i<4; i++){
		code_right_3[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	for(i=0; i<4; i++){
		code_right_4[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	for(i=0; i<4; i++){
		code_right_5[i]=EDGE_LINE_GROUP [a];
		a++;
	}
	for(i=0; i<4; i++){
		code_right_6[i]=EDGE_LINE_GROUP [a];
		a++;
	}

	



	RightDigit(1, code_right_1[0], code_right_1[1], code_right_1[2], code_right_1[3], code_right_2[0]);
	RightDigit(2, code_right_2[0], code_right_2[1], code_right_2[2], code_right_2[3], code_right_3[0]);
	RightDigit(3, code_right_3[0], code_right_3[1], code_right_3[2], code_right_3[3], code_right_4[0]);
	RightDigit(4, code_right_4[0], code_right_4[1], code_right_4[2], code_right_4[3], code_right_5[0]);
	RightDigit(5, code_right_5[0], code_right_5[1], code_right_5[2], code_right_5[3], code_right_6[0]);
	RightDigit(6, code_right_6[0], code_right_6[1], code_right_6[2], code_right_6[3], EDGE_LINE_GROUP [57]);

	LeftDigit(1, code_left_1[0], code_left_1[1], code_left_1[2], code_left_1[3], EDGE_LINE_GROUP[4] );
	LeftDigit(2, code_left_2[0], code_left_2[1], code_left_2[2], code_left_2[3], code_left_1[3] );
	LeftDigit(3, code_left_3[0], code_left_3[1], code_left_3[2], code_left_3[3], code_left_2[3] );
	LeftDigit(4, code_left_4[0], code_left_4[1], code_left_4[2], code_left_4[3], code_left_3[3] );
	LeftDigit(5, code_left_5[0], code_left_5[1], code_left_5[2], code_left_5[3], code_left_4[3] );
	LeftDigit(6, code_left_6[0], code_left_6[1], code_left_6[2], code_left_6[3], code_left_5[3] );

	printf("LEFT_NUMBER : %d,%d,%d,%d,%d,%d\n",LEFT_NUMBER[1],LEFT_NUMBER[2],LEFT_NUMBER[3],LEFT_NUMBER[4],LEFT_NUMBER[5],LEFT_NUMBER[6]);
	printf("parity_left : %c,%c,%c,%c,%c,%c\n",parity_left[1],parity_left[2],parity_left[3],parity_left[4],parity_left[5],parity_left[6]);
	printf("RIGHT_NUMBER : %d,%d,%d,%d,%d,%d\n",RIGHT_NUMBER[1],RIGHT_NUMBER[2],RIGHT_NUMBER[3],RIGHT_NUMBER[4],RIGHT_NUMBER[5],RIGHT_NUMBER[6]);
	printf("parity_right : %c,%c,%c,%c,%c,%c\n",parity_right[1],parity_right[2],parity_right[3],parity_right[4],parity_right[5],parity_right[6]);

	printf("#############################################################\n");
	//int ERROR = ErrorCheck();
	//char RightCheck = ((parity_right[1] == parity_right[2]) && (parity_right[1] == parity_right[3]) && (parity_right[1] == parity_right[4]) && (parity_right[1] == parity_right[5]) && (parity_right[1] == parity_right[6]) && (parity_right[1] == "E"));
	int NUM_13 = digit13(parity_left[1],parity_left[2],parity_left[3],parity_left[4],parity_left[5],parity_left[6]);
	printf("13 number : %d\n",NUM_13);


	cvShowImage( "Camera image", frame );
	
	}while ('q'!=cvWaitKey(10));

	// Releases the CvCapture structure
	cvReleaseCapture( &capture );
	// Destroys all the HighGUI windows
	cvDestroyAllWindows( );
	return 0;







}




void LeftDigit(int number,int LINE_1_R,int LINE_1_F,int LINE_2_R,int LINE_2_F,int beginP){
	float T1,T2,T4;
	float T = LINE_2_F-beginP;
	T1 = LINE_2_F - LINE_1_F;
	T2 = LINE_2_R - LINE_1_R;
	T4 = LINE_1_R - beginP;
	int T1_width=T_Width(T1,T);
	int T2_width=T_Width(T2,T);
	int T4_width=T_Width(T4,T);
	COMPARE_TABLE(T1_width, T2_width, T4_width , &LEFT_NUMBER[number], &parity_left[number]);
}




void RightDigit(int number,int LINE_1_R,int LINE_1_F,int LINE_2_R,int LINE_2_F,int endP){
	float T1,T2,T4;
	float T = endP - LINE_1_R;
	T1 = LINE_2_R - LINE_1_R;
	T2 = LINE_2_F - LINE_1_F;
	T4 = endP - LINE_2_F;
	int T1_width=T_Width(T1,T);
	int T2_width=T_Width(T2,T);
	int T4_width=T_Width(T4,T);
	COMPARE_TABLE(T1_width, T2_width, T4_width , &RIGHT_NUMBER[number], &parity_right[number]);
}




void COMPARE_TABLE(int T1_width, int T2_width, int T4_width ,int *character, char *parity){

	if(T1_width==2&&T2_width==2){
		*character=6;
		*parity='E';
	}
	if(T1_width==2&&T2_width==3){
		*character=0;
		*parity='O';
	}
	if(T1_width==2&&T2_width==4){
		*character=4;
		*parity='E';
	}
	if(T1_width==2&&T2_width==5){
		*character=3;
		*parity='O';
	}
	if(T1_width==3&&T2_width==2){
		*character=9;
		*parity='O';
	}
	if(T1_width==3&&T2_width==3&&T4_width==2){
		*character=2;
		*parity='E';
	}
	if(T1_width==3&&T2_width==3&&T4_width==3){
		*character=8;
		*parity='E';
	}
	if(T1_width==3&&T2_width==4&&T4_width==2){
		*character=1;
		*parity='O';
	}
	if(T1_width==3&&T2_width==4&&T4_width==1){
		*character=7;
		*parity='O';
	}
	if(T1_width==3&&T2_width==5){
		*character=5;
		*parity='E';
	}
	if(T1_width==4&&T2_width==2){
		*character=9;
		*parity='E';
	}
	if(T1_width==4&&T2_width==3&&T4_width==2){
		*character=2;
		*parity='O';
	}
	if(T1_width==4&&T2_width==3&&T4_width==1){
		*character=8;
		*parity='O';
	}
	if(T1_width==4&&T2_width==4&&T4_width==1){
		*character=1;
		*parity='E';
	}
	if(T1_width==4&&T2_width==4&&T4_width==2){
		*character=7;
		*parity='E';
	}
	if(T1_width==4&&T2_width==5){
		*character=5;
		*parity='O';
	}
	if(T1_width==5&&T2_width==2){
		*character=6;
		*parity='O';
	}
	if(T1_width==5&&T2_width==3){
		*character=0;
		*parity='E';
	}
	if(T1_width==5&&T2_width==4){
		*character=4;
		*parity='O';
	}
	if(T1_width==5&&T2_width==5){
		*character=3;
		*parity='E';
	}
}



int T_Width(float Ti, float T)
{
	int width=0;

	//cout<<"Ti divide by T is "<<Ti<<"/"<<T<<"="<<Ti/T<<endl;
	//cout<<"Ti divide by T is "<<3<<"/"<<14<<"="<<3.0/14<<endl;
	
	if((Ti/T)<=(3.0/14)){
		width=1;
	}
	if((3.0/14)<(Ti/T)&&(Ti/T)<=(5.0/14)){
	width=2;
	}
	if((5.0/14)<(Ti/T)&&(Ti/T)<=(7.0/14)){
	width=3;
	}
	if((7.0/14)<(Ti/T)&&(Ti/T)<=(9.0/14)){
	width=4;
	}
	if((9.0/14)<(Ti/T)&&(Ti/T)<=(11.0/14)){
	width=5;
	}
	//cout<<"width is "<<width<<endl;
	return width;
}


int digit13(char par1,char par2,char par3,char par4,char par5,char par6)
{
	//the 1st parity will always be zero, so ignore 1st parity
	if((par2=='O')&&(par3=='O')&&(par4=='O')&&(par5=='O')&&(par6=='O'))
	{
		return 0;
	}
	if((par2=='O')&&(par3=='E')&&(par4=='O')&&(par5=='E')&&(par6=='E'))
	{
		return 1;
	}
	if((par2=='O')&&(par3=='E')&&(par4=='E')&&(par5=='O')&&(par6=='E'))
	{
		return 2;
	}
	if((par2=='O')&&(par3=='E')&&(par4=='E')&&(par5=='E')&&(par6=='O'))
	{
		return 3;
	}
	if((par2=='E')&&(par3=='O')&&(par4=='O')&&(par5=='E')&&(par6=='E'))
	{
		return 4;
	}
	if((par2=='E')&&(par3=='E')&&(par4=='O')&&(par5=='O')&&(par6=='E'))
	{
		return 5;
	}
	if((par2=='E')&&(par3=='E')&&(par4=='E')&&(par5=='O')&&(par6=='O'))
	{
		return 6;
	}
	if((par2=='E')&&(par3=='O')&&(par4=='E')&&(par5=='O')&&(par6=='E'))
	{
		return 7;
	}
	if((par2=='E')&&(par3=='O')&&(par4=='E')&&(par5=='E')&&(par6=='O'))
	{
		return 8;
	}
	if((par2=='E')&&(par3=='E')&&(par4=='O')&&(par5=='E')&&(par6=='O'))
	{
		return 9;
	}
}
