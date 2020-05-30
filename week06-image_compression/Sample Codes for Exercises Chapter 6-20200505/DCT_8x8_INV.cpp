//A very simple cosine transform for blocks of images
//This code is O(N^2), there are more efficient ways to compute DCTs
//OpenCV has faster DCT transform implementations
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

#define PI 3.14159265358979323846264338327950288419716939937510582097494459230
#define N 8
main(){
  //coefficients
  //image recovery
  float I[N][N]= {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0};

  //quantized coefficients
  int R[N][N]= {765,-24,-282,-32,-237,0,-118,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0};
  //decompressed image
  int DECOMPIMAGE[N][N]=
  {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0};
  float alphaU0=sqrt(1.0/N);
  float alphaU=sqrt(2.0/N);
  float alphaa=0,alphab=0;
  //get only a few coefficients
  //recover image from coefficients
  for (int x=0;x<N;x++){
    for (int y=0;y<N;y++){
      for(int a=0;a<N;a++){
        for(int b=0;b<N;b++){
         if (a==0) alphaa=alphaU0;
         else alphaa=alphaU;
         if (b==0) alphab=alphaU0;
         else alphab=alphaU;
          I[x][y]=(alphaa*alphab*R[a][b]*cos(((2*x+1)*a*PI)/(2*N))*cos(((2*y+1)*b*PI)/(2*N))) +I[x][y];
          //printf("C[%d %d] %1.3f \n",a,b,C[a][b]);
        }
      }
      DECOMPIMAGE[x][y]=(int) round(I[x][y]);
    }
  }

  for (int a=0;a<N;a++){
    for (int b=0;b<N;b++){
    //printf("f[%d,%d] %d ",a,b,DECOMPIMAGE[a][b]);
      printf("%d  ",DECOMPIMAGE[a][b]);
      if (((b+1)%N)==0) printf("\n");
    }
  }
}