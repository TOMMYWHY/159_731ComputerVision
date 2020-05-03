//159731 - Computer Vision
// A simple implementation of a labelling algorithm
// by Andre Barczak (2010).
//simple HSV filter for logitech cameras
//
#include "cv.h"
#include "highgui.h"
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

const int upB=13;
const int loB=0;
const int upG=255;
const int loG=205;
const int upR=139;
const int loR=80;
//
int marker_upB=upB;
int marker_loB=loB;
int marker_upG=upG;
int marker_loG=loG;
int marker_upR=upR;
int marker_loR=loR;


int iKey=0;
int npflag=0;

void Find_markers(IplImage* image, IplImage *imageBGR){
int debugcount=0;
	cvCopyImage(image,imageBGR);
//segment markers
	for (int x=0;x<imageBGR->width;x++){
		for (int y=0;y<imageBGR->height;y++){
			if( pixelR(imageBGR,x,y) < marker_loR || pixelR(imageBGR,x,y) > marker_upR ||
			pixelG(imageBGR,x,y) < marker_loG || pixelG(imageBGR,x,y) > marker_upG ||
			pixelB(imageBGR,x,y) < marker_loB || pixelB(imageBGR,x,y) > marker_upB   ){
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
    CvCapture* capture = 0;
    IplImage *imagecaptured;
    IplImage *imagemarkers;
    IplImage *imageBGR;
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
            imageBGR = cvCreateImage( cvSize(imagecaptured->width,imagecaptured->height), 8, 3 );
        }
//HSV controls
cvCreateTrackbar( "upB", "result", &marker_upB, 255, NULL);
cvSetTrackbarPos("upB","result",upB);
cvCreateTrackbar( "loB", "result", &marker_loB, 255, NULL);
cvSetTrackbarPos("loB","result",loB);

cvCreateTrackbar( "upG", "result", &marker_upG, 255, NULL);
cvSetTrackbarPos("upG","result",upG);
cvCreateTrackbar( "loG", "result", &marker_loG, 255, NULL);
cvSetTrackbarPos("loG","result",loG);

cvCreateTrackbar( "upR", "result", &marker_upR, 255, NULL);
cvSetTrackbarPos("upR","result",upR);
cvCreateTrackbar( "loR", "result", &marker_loR, 255, NULL);
cvSetTrackbarPos("loR","result",loR);
	
        for(;;)
        {
//controls the device 
	    control_capture(capture);
            if( !cvGrabFrame( capture ))
                break;
            imagecaptured = cvRetrieveFrame( capture );
            
            if( !imagecaptured )
                break;
	    //KEYS marker_upB
		iKey=cvWaitKey(2);
		switch(iKey)
		{
			case 113:
			exit(0);
		}
//            cvResetImageROI(imageBGR);
  //          cvSetImageROI(imageBGR, cvGetImageROI(imagecaptured));
            imagemarkers=cvCloneImage(imagecaptured);
//after ROI, find markers
            Find_markers(imagemarkers,imageBGR);
	    cvShowImage( "result", imagemarkers );
	    cvShowImage( "captured image", imagecaptured );
            cvSetZero(imageBGR);
            cvReleaseImage(&imagemarkers);
//cvWaitKey(0);
        }
        cvReleaseImage( &imagecaptured);
        cvReleaseCapture( &capture );
    }
    cvDestroyWindow("result");
    cvDestroyWindow("captured image");
    return 0;
}

