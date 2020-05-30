#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "cv.h"
#include "highgui.h"

#include "lookuptables_colour.c"


IplImage* src;// = cvCreateImage( cvSize(MAXCOLS,MAXROWS) , IPL_DEPTH_8U , 1 );
IplImage* dst = cvCreateImage( cvSize(640,240) , IPL_DEPTH_8U , 3 );
IplImage* panorextra = cvCreateImage( cvSize(dst->width+120,dst->height) , IPL_DEPTH_8U , 3 );


//IplImage* per = cvCreateImage( cvSize(100,100) , IPL_DEPTH_8U , 1 );
IplImage* per = cvCreateImage( cvSize(100,100) , IPL_DEPTH_8U , 3 );
IplImage* per2 = cvCreateImage( cvSize(200,200) , IPL_DEPTH_8U , 3 );
IplImage* per_from_omnidir = cvCreateImage( cvSize(100,100) , IPL_DEPTH_8U , 3 );

float dist=170;//external "f" variable for zooming in/out



/*#define CV_EVENT_MOUSEMOVE      0
#define CV_EVENT_LBUTTONDOWN    1
#define CV_EVENT_RBUTTONDOWN    2
#define CV_EVENT_MBUTTONDOWN    3
#define CV_EVENT_LBUTTONUP      4
#define CV_EVENT_RBUTTONUP      5
#define CV_EVENT_MBUTTONUP      6
#define CV_EVENT_LBUTTONDBLCLK  7
#define CV_EVENT_RBUTTONDBLCLK  8
#define CV_EVENT_MBUTTONDBLCLK  9

#define CV_EVENT_FLAG_LBUTTON   1
#define CV_EVENT_FLAG_RBUTTON   2
#define CV_EVENT_FLAG_MBUTTON   4
#define CV_EVENT_FLAG_CTRLKEY   8
#define CV_EVENT_FLAG_SHIFTKEY  16
#define CV_EVENT_FLAG_ALTKEY    32
*/
void on_mouse_omniview(int event,int x,int y,int flag,void*bla)
{
if(event==1)//left click
{
printf("pos %d %d \n",x,y);
compute_lookuptables(lookup_ang,lookup_t,x,y);
//redraw panoramicas
compute_panoramica_colour(src, dst);
cvSetImageROI(dst, cvRect(0,0,120,240));
cvSetImageROI(panorextra, cvRect(639,0,120,240));
cvCopy(dst,panorextra);
cvResetImageROI(dst);
cvResetImageROI(panorextra);
cvSetImageROI(panorextra, cvRect(0,0,640,240));
cvCopy(dst,panorextra);
cvResetImageROI(panorextra);
cvShowImage( "Panoramica" , dst );
cvShowImage( "Panorextra" , panorextra );
}
}

void on_mouse_locatepixels(int event,int x,int y,int flag,void*bla)
{
float phi_z, theta_z;
theta_z=(2*3.141592105*(float)(640-x))/640;
phi_z=-1.3*((float)(y-60)/240);

if(event==1)//left click
{
printf("pos %d %d \n",x,y);
uchar *pixeld=&((uchar*)(panorextra->imageData+panorextra->widthStep*0))[0];
pixeld[y*panorextra->widthStep+x*3]=255;
pixeld[(y*panorextra->widthStep+x*3)+1]=0;
pixeld[(y*panorextra->widthStep+x*3)+2]=0;


printf("angle %f %f %f \n",theta_z, phi_z, dist);
compute_perspective_colour(src, per, phi_z, theta_z, dist);

cvShowImage( "Per" , per );
cvShowImage( "Panorextra" , panorextra );
}

}
void on_mouse(int event,int x,int y,int flag,void*bla)
{
float phi_z, theta_z;
//Incorrect, REWRITE THIS
// theta based on x is correct,
//y should be computed through a lookup table
theta_z=(2*3.141592105*(float)(640-x))/640;
//(lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + (int)((pos_x*MTHETA)/dst->width)]
//phi_z=-1.3*((float)(y-60)/240);
phi_z=-1.3*((float)(y-80)/240);
//use MPHI=605
//phi_z=(float)((y*605)/240);//DOES NOT WORK

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

printf("pos %d %d \n",x,y);
printf("angle theta %f phi %f dist %f \n",theta_z, phi_z, dist);
compute_perspective_colour(src, per, phi_z, theta_z, dist);
//detect_and_draw(per );
cvShowImage( "Per" , per );
cvResize( per, per2, CV_INTER_LINEAR );
//cvPyrUp(per,per2, CV_GAUSSIAN_5x5 );
cvShowImage( "Per_fromOmnidir" , per2 );
}

void on_mouse_fromOmnidir(int event,int x,int y,int flag,void*bla)
{
//if (event==1)//left click
float phi_z, theta_z;
//float f=170;
if (event==0)//mouse moves
{
//printf("pos %d %d \n",x,y);
theta_z=(2*3.141592105*(float)x)/640;
phi_z=-1.3*((float)(y-60)/240);

//if(phi_z>0.3)phi_z=0.3;
if(phi_z>0.3)phi_z=0.3;
printf("angle theta %f phi %f dist %f \n",theta_z, phi_z, dist);
compute_perspective_colour(src, per, phi_z, theta_z, dist);
//detect_and_draw(per );
cvShowImage( "Per" , per );
}
if(event==1)//left click
{
printf("pos %d %d \n",x,y);
theta_z=(2*3.141592105*(float)x)/640;
//phi_z=-1.3*((float)(y-60)/240);
phi_z=-1.3*((float)(y-60)/240);
if(phi_z>0.3)phi_z=0.3;
dist=dist+10;//left click zoom in
//if ( (dist >=170) && (dist <= 1000) ) dist=dist+10;//left click zoom in
//else dist=dist+100;
printf("angle %f %f %f \n",theta_z, phi_z, dist);
compute_perspective_colour(src, per, phi_z, theta_z, dist);
//detect_and_draw(per );
cvShowImage( "Per" , per );
}
if(event==2)//right click
{
printf("pos %d %d \n",x,y);
theta_z=(2*3.141592105*(float)x)/640;
phi_z=-1.3*((float)(y-60)/240);
if(phi_z>0.3)phi_z=0.3;
dist=dist-10;//right click zoom out
if (dist<170) dist=170;
printf("angle %f %f %f \n",theta_z, phi_z, dist);
compute_perspective_colour(src, per, phi_z, theta_z, dist);
//detect_and_draw(per );
cvShowImage( "Per" , per );
}
/*if(event==3)printf("e3\n");
if(event==6)printf("e6\n");
if(event==9)printf("e9\n");
if(flag==4)printf("f4\n");
if(flag==8)printf("f8\n");
if(flag==16)printf("f16\n");
if(flag==32)printf("f32\n");*/
}

main(int argc, char** argv){
if(argc!=2){printf("needs an omnidirectional image \n");exit(0);}

cvNamedWindow( "OmniView" , CV_WINDOW_AUTOSIZE );
cvNamedWindow( "Panoramica" , CV_WINDOW_AUTOSIZE );
cvNamedWindow( "Panorextra" , CV_WINDOW_AUTOSIZE );
cvNamedWindow( "Per" ,CV_WINDOW_AUTOSIZE );
cvNamedWindow( "Per_fromOmnidir" ,CV_WINDOW_AUTOSIZE );
compute_lookuptables(lookup_ang,lookup_t,320,240);//initial guess for centre
printf("loading image...\n");
src=cvLoadImage(argv[1], 1);
printf("computing panoramic 1...\n");
compute_panoramica_colour(src, dst);
printf("computing panoramic 2...\n");
cvSetImageROI(dst, cvRect(0,0,120,240));
printf("computing panoramic 3...\n");
cvSetImageROI(panorextra, cvRect(639,0,120,240));
printf("computing panoramic 4...\n");
cvCopy(dst,panorextra);
printf("computing panoramic 5...\n");
cvResetImageROI(dst);
printf("computing panoramic 6...\n");
cvResetImageROI(panorextra);
printf("computing panoramic 7...\n");
cvSetImageROI(panorextra, cvRect(0,0,640,240));
printf("computing panoramic 8...\n");
cvCopy(dst,panorextra);
printf("computing panoramic 9...\n");
cvResetImageROI(panorextra);
printf("computing panoramic...\n");

printf("showing images ...\n");
cvShowImage( "OmniView" , src );
cvShowImage( "Panoramica" , dst );
cvShowImage( "Panorextra" , panorextra );
//compute_perspective(src, per, phi_z, theta_z));

/*
	uchar *pixels=&((uchar*)(src->imageData+src->widthStep*0))[0];
	uchar *pixeld=&((uchar*)(per->imageData+dst->widthStep*0))[0];
	int largs=src->widthStep;
	int largd=per->widthStep;
	for(int pos_y=0;pos_y<per->height;pos_y++){
		for(int pos_x=0;pos_x<per->width;pos_x++){
//			pixeld[pos_y*larg+pos_x]=pixels[(src->height-pos_y)*larg+pos_x];
			pixeld[pos_y*largd+pos_x]=pixels[pos_y*largs+pos_x];
		}
	}
*/
//printf("larg %d %d %d \n",src->widthStep,dst->widthStep,per->widthStep);
//cvResizeWindow( "Per", 200, 200);
cvShowImage( "Per" , per );
cvShowImage( "Per_fromOmnidir" , per2 );
cvSetMouseCallback("Panoramica",on_mouse);
cvSetMouseCallback("OmniView",on_mouse_omniview);
cvSetMouseCallback("Panorextra",on_mouse_locatepixels);
//cvShowImage( "Per" , per );
cvWaitKey(0);
}
