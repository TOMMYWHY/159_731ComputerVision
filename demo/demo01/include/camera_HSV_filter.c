//159731 - Computer Vision
// A simple implementation of a labelling algorithm
// by Andre Barczak (2010).
//simple HSV filter for logitech cameras
//
//#include "cv.h"
#include <opencv2/opencv.hpp>

//#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

#include "controls_logitech.c"

//Macros for colour pixels
#define pixelB(image,x,y) \
((uchar *)(image->imageData + (y)*image->widthStep))[(x)*image->nChannels]
#define pixelG(image,x,y) \
((uchar *)(image->imageData + (y)*image->widthStep))[(x)*image->nChannels+1]
#define pixelR(image,x,y) \
((uchar *)(image->imageData + (y)*image->widthStep))[(x)*image->nChannels+2]

const int upH=85;
const int loH=60;
const int upS=255;
const int loS=190;
const int upV=255;
const int loV=200;
//
int marker_upH=upH;
int marker_loH=loH;
int marker_upS=upS;
int marker_loS=loS;
int marker_upV=upV;
int marker_loV=loV;


int iKey=0;
int npflag=0;



void Find_markers(IplImage* image, IplImage *imageHSV){
int debugcount=0;
//convert to HSV, the order is the same as BGR
	cvtColor(image,imageHSV, CV_RGB2HSV);
//segment markers
	for (int x=0;x<imageHSV->width;x++){
		for (int y=0;y<imageHSV->height;y++){
			if( pixelR(imageHSV,x,y) < marker_loV || pixelR(imageHSV,x,y) > marker_upV ||
			pixelG(imageHSV,x,y) < marker_loS || pixelG(imageHSV,x,y) > marker_upS ||
			pixelB(imageHSV,x,y) < marker_loH || pixelB(imageHSV,x,y) > marker_upH   ){
				pixelR(image,x,y)=0;
				pixelG(image,x,y)=0;
				pixelB(image,x,y)=0;
			}	
			else {
				pixelR(image,x,y)=255;
				pixelG(image,x,y)=255;
				pixelB(image,x,y)=255;
				debugcount++;
			}
		}
	}
}




int main( int argc, char** argv )
{
    VideoCapture* capture = 0;
    IplImage *imagecaptured;
    IplImage *imagemarkers;
    IplImage *imageHSV;
    capture = cvCaptureFromCAM(0);
    cvNamedWindow( "captured image", 0 );
    cvNamedWindow( "result", 1 );

    if( capture )
    {
cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 960);//doesn't work with opencv2.0?
printf("%f \n",cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH));
cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 700);//doesn't work with opencv2.0???
printf("%f \n",cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT));
startup_controls(capture,0);
printf("Startup ok\n");

//first capture for getting sizes
        if( cvGrabFrame( capture )) {
            imagecaptured = cvRetrieveFrame( capture );
printf("width %d height %d \n",imagecaptured->width,imagecaptured->height);
            imageHSV = cvCreateImage( cvSize(imagecaptured->width,imagecaptured->height), 8, 3 );
        }
//HSV controls
cvCreateTrackbar( "upH", "result", &marker_upH, 255, NULL);
cvSetTrackbarPos("upH","result",upH);
cvCreateTrackbar( "loH", "result", &marker_loH, 255, NULL);
cvSetTrackbarPos("loH","result",loH);

cvCreateTrackbar( "upS", "result", &marker_upS, 255, NULL);
cvSetTrackbarPos("upS","result",upS);
cvCreateTrackbar( "loS", "result", &marker_loS, 255, NULL);
cvSetTrackbarPos("loS","result",loS);

cvCreateTrackbar( "upV", "result", &marker_upV, 255, NULL);
cvSetTrackbarPos("upV","result",upV);
cvCreateTrackbar( "loV", "result", &marker_loV, 255, NULL);
cvSetTrackbarPos("loV","result",loV);
	
        for(;;)
        {
//controls the device 
	    control_capture(capture);
            if( !cvGrabFrame( capture ))
                break;
            imagecaptured = cvRetrieveFrame( capture );
            if( !imagecaptured )
                break;
		iKey=cvWaitKey(2);
		if(iKey==113) exit(0);//if key==q
            imagemarkers=cvCloneImage(imagecaptured);
            Find_markers(imagemarkers,imageHSV);
	    cvShowImage( "result", imagemarkers );
	    cvShowImage( "captured image", imagecaptured );
            cvSetZero(imageHSV);
            cvReleaseImage(&imagemarkers);
        }
        cvReleaseImage( &imagecaptured);
        cvReleaseCapture( &capture );
    }
    cvDestroyWindow("result");
    cvDestroyWindow("captured image");
    return 0;
}

