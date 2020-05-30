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
  float C[N][N]= {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0};
  //image recovery
  float I[N][N]= {0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0};

  //IMAGE: uncomment and comment the appropriate lines to change the original values
  //image, repeated values are represented by fewer coefficients
  int IMAGE[N][N]=
  {1,127,127,120,120,123,124,23,
  1,127,127,120,120,123,124,23,
  1,127,127,120,120,123,124,23,
  1,127,127,120,120,123,124,23,
  1,127,127,120,120,123,124,23,
  1,127,127,120,120,123,124,23,
  1,127,127,120,120,123,124,23,
  1,127,127,120,120,123,124,23};
  //alternative image, note that all coefficients are different than zero
  /*int IMAGE[N][N]=
  {1,2,67,12,127,200,24,123,
  32,27,1,12,10,123,14,223,
  45,17,7,10,14,123,24,23,
  57,17,2,2,15,123,1,43,
  78,17,1,1,180,123,2,26,
  78,27,2,0,190,123,4,63,
  98,127,7,0,110,123,24,83,
  100,127,17,0,11,123,4,28};*/

  //quantized coefficients
  int R[N][N]= {0,0,0,0,0,0,0,0,
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
  for (int a=0;a<N;a++){
    for (int b=0;b<N;b++){
      if (a==0) alphaa=alphaU0;
      else alphaa=alphaU;
      if (b==0) alphab=alphaU0;
      else alphab=alphaU;
      for(int x=0;x<N;x++){
        for(int y=0;y<N;y++){
          C[a][b]=(alphaa*alphab*IMAGE[x][y]*cos(((2*x+1)*a*PI)/(2*N))*cos(((2*y+1)*b*PI)/(2*N))) +C[a][b];
          //printf("C[%d %d] %1.3f \n",a,b,C[a][b]);
        }
      }
      //rounding up values for R(u,v)
      R[a][b]=(int)round(C[a][b]);
    }
  }
  for (int a=0;a<N;a++){
    for (int b=0;b<N;b++){
    //printf("C[%d,%d] %1.5f ",a,b,C[a][b]);
      printf("%1.5f       ",C[a][b]);
      if (((b+1)%N)==0) printf("\n");
    }
  }
  //printf("\n alpha %f %f %f %f \n",alphaU, alphaU0, alphaa, alphab);
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
  ofstream outputfile;
  outputfile.open("Cosine8x8.bin", ofstream::binary|ios::out);
  if(!outputfile.good()){printf("Can't open the output file \n");return 0;}
  
  for (int a=0;a<N;a++){
    for (int b=0;b<N;b++){
      if( C[a][b] > 0.001 || C[a][b]< -0.001){
	outputfile.write((char *)&C[a][b],sizeof(C[a][b]));
	printf("%1.5f       ",C[a][b]);
      }
    }
  }
  outputfile.close();
}