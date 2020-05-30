#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>

#include "lookuptables_colour.c"

//IplImage* src;
IplImage* frame = 0;
IplImage* dst = cvCreateImage( cvSize(640,240) , IPL_DEPTH_8U , 3 );
//IplImage* panorextra = cvCreateImage( cvSize(dst->width+120,dst->height) , IPL_DEPTH_8U , 3 );
IplImage* per = cvCreateImage( cvSize(100,100) , IPL_DEPTH_8U , 3 );
//for perspective view
float phi_z, theta_z;
float dist=170;//external "f" variable for zooming in/out

void on_mouse_findcentre(int event,int x,int y,int flag,void*bla)
{
if(event==1)//left click
{
compute_lookuptables(lookup_ang,lookup_t,x,y);
}

}

void on_mouse(int event,int x,int y,int flag,void*bla)
{
//y should be computed through a lookup table
theta_z=(2*3.141592105*(float)(640-x))/640;
phi_z=-1.3*((float)(y-80)/240);
if (event==0)//mouse moves
{
}
if(event==1)//left click
{
dist=dist+10;//left click zoom in
}
if(event==2)//right click
{
dist=dist-10;//right click zoom out
}
//printf("pos %d %d \n",x,y);
//printf("angle theta %f phi %f dist %f \n",theta_z, phi_z, dist);
compute_perspective_colour(frame, per, phi_z, theta_z, dist);
//detect_and_draw(per );
cvShowImage( "Per" , per );
//cvResize( per, per2, CV_INTER_LINEAR );
//cvPyrUp(per,per2, CV_GAUSSIAN_5x5 );
//cvShowImage( "Per_fromOmnidir" , per2 );
}


int main( int argc, char** argv )
{
compute_lookuptables(lookup_ang,lookup_t,320,240);//initial guess for centre

    CvCapture* capture = 0;
    capture = cvCaptureFromCAM(0);
    if (capture == NULL) {printf("did you connect the camera??\n"); exit(0);}
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH,640);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT,480);
    

cvNamedWindow( "OmniView" , 1);
cvNamedWindow( "Panoramica" ,1);
cvNamedWindow( "Per" ,0);
    while(1)
    {
        
        frame = cvQueryFrame( capture );
        cvShowImage( "OmniView", frame );
cvSetMouseCallback("OmniView",on_mouse_findcentre);
compute_panoramica_colour(frame, dst);
        cvShowImage( "Panoramica", dst );
cvSetMouseCallback("Panoramica",on_mouse);
compute_perspective_colour(frame, per, phi_z, theta_z, dist);
cvShowImage( "Per" , per );

        //printf("frame size %d %d\n",frame->width,frame->height);
        //printf("%d \n",cvWaitKey(10));
int key;

key=cvWaitKey(30);
//printf("key %d \n",key);
//if (cvWaitKey(30) == 1048689) break;
if (key == 27) break;
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow("OmniView");
    cvDestroyWindow("Panoramica");
    cvDestroyWindow("Per");
    return 0;
} 
