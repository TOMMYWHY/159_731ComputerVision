//
// Created by Tommy on 2020/6/5.
//




#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<algorithm>
#include <math.h>

#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]



#if 1
void EllipticFourierDescriptors(vector<Point>& contour , vector< float> CE) {
    vector<float> ax, ay, bx, by;
    int m = contour.size();
    int n = 20;//number of CEs we are interested in computing
     float t=(2*M_PI)/m;
    for (int k = 0; k < n; k++) {
        ax.push_back(0.0);
        ay.push_back(0.0);
        bx.push_back(0.0);
        by.push_back(0.0);
        for (int i = 0; i < m; i++) {
            ax[k] = ax[k] + contour[i].x * cos((k + 1) * t * (i));
            bx[k] = bx[k] + contour[i].x * sin((k + 1) * t * (i));
            ay[k] = ay[k] + contour[i].y * cos((k + 1) * t * (i));
            by[k] = by[k] + contour[i].y * sin((k + 1) * t * (i));
        }
        ax[k] = (ax[k]) / m;
        bx[k] = (bx[k]) / m;
        ay[k] = (ay[k]) / m;
        by[k] = (by[k]) / m;
    }
    for (int k = 0; k < n; k++) {
        CE.push_back(sqrt((ax[k] * ax[k] + ay[k] * ay[k]) / (ax[0] * ax[0] + ay[0] * ay[0])) +
             sqrt((bx[k] * bx[k] + by[k] * by[k]) / (bx[0] * bx[0] + by[0] * by[0])));
    }
    for (int count = 0; count < n && count < CE.size(); count++) {
        printf("%d CE %f ax %f ay %f bx %f by%f \n" ,count, CE[count], ax[count], ay[count], bx[count], by[count] );
    }
}


int main (){

//    Mat img = imread("./images/0_A.jpg",1);
    Mat img = imread("./images/1_B.jpg",1);
//    imshow("org_img",img);
    Mat gray_img, binary_img;
//    GaussianBlur(img,img,Size(3,3),0,0);

    blur(img,img, Size(3,3));
    cvtColor ( img , gray_img , COLOR_BGR2GRAY ) ;
    threshold ( gray_img , binary_img , 225 , 255 , THRESH_BINARY ) ;//todo
//    threshold(img, img, 150, 255, THRESH_BINARY_INV);

//    imshow("Binary",binary_img);

    Mat reverse_img = 255 - binary_img;
//    imshow("reverse_img",reverse_img);

    Mat obj_img;
    img.copyTo(obj_img);

    for (int x = 0; x < obj_img.cols; x++) {
        for (int y = 0; y < obj_img.rows; y++) {
            if(int(Mpixel(reverse_img ,x,y))==0){
                MpixelB(obj_img ,x,y)= 0;
                MpixelG(obj_img ,x,y)= 0;
                MpixelR(obj_img ,x,y)= 0;
            }
        }
    }
    imshow("obj_img",obj_img);
    Mat obj_gray_img,obj_binary_img;
    cvtColor( obj_img , obj_gray_img , COLOR_BGR2GRAY );
    threshold ( obj_gray_img , obj_binary_img , 5 , 255 , THRESH_BINARY ) ;
    vector<vector<Point> > contours ;
    findContours ( obj_binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE) ;
    Mat drawing = Mat::zeros( obj_binary_img.size(), CV_8UC3 );
    Scalar color = CV_RGB( 0, 255,0 );

    int largestcontour=0;
    long int largestsize=0;
    vector< vector<Point> > filterContours;	// 筛选后的轮廓
    vector< Point > hull;	// 凸包络的点集

    for(int i = 0; i< contours.size(); i++ ) {
        /*if(largestsize < contours[i].size()) {
            largestsize=contours [ i ]. size () ; largestcontour=i ;
        }*/
        if (fabs(contourArea(Mat(contours[i]))) > 30000)	//判断手进入区域的阈值
        {
            filterContours.push_back(contours[i]);
        }

    }
//    drawContours( drawing, contours, largestcontour, color, 1, 8);
    drawContours(drawing, filterContours, -1, Scalar(0,0,255), 1);


    imshow("drawing" , drawing ) ;

    vector<float> CE;
    EllipticFourierDescriptors(contours[largestcontour], CE);

    waitKey(0);

    return 0;
}
#endif