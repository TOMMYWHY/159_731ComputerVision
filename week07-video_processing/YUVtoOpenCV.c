//Program to read raw YUV (4:1:1) and convert it to BGR readable in OpenCV
//by A. Barczak 2006 Massey University NZ
//Usage: YUVtoOpenCV sequence_name.yuv x_size y_size
//e.g. YUVtoOpenCV assignment3.yuv 360 288
//Each frame is loaded into "image", so further processing should be done
//based on that. Please pay attention to the size, depth and nChannels of the
//image. Convert if necessary (it will depend on the method you use).
// Robot sequence; 640 480
// Football sequence: 640 480
// Garden sequence; 640 480
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int u,v,y_raw;
int topixel(int d) {
  if (d>254) return 255;
  if (d<1) return 0;
  return (int) d;
}


int YUVtoR(int y, int u, int v){
  int r;
  r=topixel((int)((y-16)*1.164+1.596*(v-128)));
  return (r);
}
int YUVtoG(int y, int u, int v){
  int g;
  g=topixel((int)((y-16)*1.164-0.392*(u-128)-0.813*(v-128)));
  return (g);
}
int YUVtoB(int y, int u, int v){
  int b;
  b=topixel((int)((y-16)*1.164+2.017*(u-128)));
  return (b);
}

void convertYUVtoBGR(unsigned char *imagedata, unsigned char *buffer, int width, int height) {
  int i=0;
  for (i = 0; i < width * height; i++) {
    y_raw=buffer[i];
    u=buffer[(width*height)+((((i/width)/2)*width)/2)+((i%width)/2)];
    v=buffer[(width*height)+(int)(0.25*width*height)+((((i/width)/2)*width)/2)+((i%width)/2)];
    imagedata[i*3]=YUVtoB(y_raw,u,v);
    imagedata[(i*3)+1]=YUVtoG(y_raw,u,v);
    imagedata[(i*3)+2]=YUVtoR(y_raw,u,v);
  }

}

int winwidth=640;
int winheight=480;
FILE *fd;
unsigned char *rawimage, *rawimageBGR;
int counting=0,frames=0;
int main(int argc,char *argv[])
{
  if(argc != 4) {
    printf("usage: readYUVOpenCV <filename> <width> <height>\n\n");
    exit(0);
  }
  if(argc == 4) {
    winwidth=atoi(argv[2]);
    winheight=atoi(argv[3]);
  }
  fd=fopen(argv[1],"r");
  if(!fd) { fprintf(stderr,"can't open the file...\n"); exit(0);}
  IplImage *image,*previous;
  rawimage=(unsigned char *)malloc(int (winwidth*winheight*3)/2);
  rawimageBGR=(unsigned char *)malloc(int (winwidth*winheight*3));
  cvNamedWindow("Current frame",0);
  cvNamedWindow("Previous frame",0);
  counting=0;
  previous=cvCreateImage(cvSize(winwidth,winheight), 8, 3);
  while(cvWaitKey(1)==-1)
  {
    image=cvCreateImage(cvSize(winwidth,winheight), 8, 3);
    if (fread(rawimage, (int) ((winwidth * winheight *3 /2)),1,fd) == 0) {
      cvReleaseImage(&image);
      cvDestroyWindow("Current frame");
      free(rawimage);free(rawimageBGR);
      printf("End of the sequence. \n");
      exit(0);
    }
    frames=frames+1;
    printf("frame %d \n",frames);
    cvWaitKey(0);//Comment this line to see the sequence as a movie
    convertYUVtoBGR(rawimageBGR,rawimage,winwidth,winheight);
    memcpy((unsigned char *)image->imageData,rawimageBGR,winwidth*winheight*3);
    cvShowImage("Current frame",image);
    cvShowImage("Previous frame",previous);
        /*********further processing here *****************/
    cvCopyImage(image,previous);
    cvReleaseImage(&image);
  }
cvWaitKey(0);
  cvReleaseImage(&image);
  cvDestroyWindow("Current frame");
  free(rawimage);
  free(rawimageBGR);
  printf("Exit (finished by the user). \n\n");
  return 0;
}
