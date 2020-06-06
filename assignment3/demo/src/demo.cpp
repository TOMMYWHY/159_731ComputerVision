//
// Created by Tommy on 2020/6/5.
//




#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<algorithm>
#include <math.h>

#include<stdlib.h>
#include<stdio.h>

#include "LoadData.h"


using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]



#if 01
struct Feature{
    int index;
    double value;
};
vector<Feature> EllipticFourierDescriptors(vector<Point>& contour , vector< float> CE) {
    vector<float> ax, ay, bx, by;
    vector<Feature> feature_list;
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
//        printf("%d CE %f ax %f ay %f bx %f by%f \n" ,count, CE[count], ax[count], ay[count], bx[count], by[count] );
//        cout <<"CE index:"<<count <<"; CE value:"<< CE[count] <<endl;
        Feature feature;
        feature.index = count;
        feature.value = CE[count];
        feature_list.push_back(feature);
    }

    return feature_list;

}

Mat get_obj_img(Mat org_img){
    Mat gray_img, binary_img,obj_img;
    blur(org_img,org_img, Size(3,3));
    cvtColor ( org_img , gray_img , COLOR_BGR2GRAY ) ;
    threshold ( gray_img , binary_img , 225 , 255 , THRESH_BINARY ) ;//todo 区域划分不精准
    //    imshow("Binary",binary_img);

    Mat reverse_img = 255 - binary_img;
    org_img.copyTo(obj_img);
    for (int x = 0; x < obj_img.cols; x++) {
        for (int y = 0; y < obj_img.rows; y++) {
            if(int(Mpixel(reverse_img ,x,y))==0){
                MpixelB(obj_img ,x,y)= 0;
                MpixelG(obj_img ,x,y)= 0;
                MpixelR(obj_img ,x,y)= 0;
            }
        }
    }
//    imshow("obj_img",obj_img);
    return obj_img;
}

vector<Feature> get_feature(Mat obj_img){
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
    drawContours(drawing, filterContours, -1, Scalar(0,0,255), 1);
    imshow("drawing" , drawing ) ;
    vector<float> CE;
    vector<Feature> feature_list = EllipticFourierDescriptors(contours[largestcontour], CE);
    return feature_list;
}

int main (){

    LoadData test;
    test.get_files_name();

//    Mat img = imread("./images/0_A.jpg",1);
    Mat img = imread("./images/1_B.jpg",1);
//    imshow("org_img",img);
    Mat obj_img = get_obj_img(img);
    vector<Feature> feature_list = get_feature(obj_img);

    vector<vector<Feature>> feature_list_all;
    feature_list_all.push_back(feature_list);

/* 写 文件*/

    ofstream w_file;
    w_file.open("./res/test.data",ios::app);
    for (int i = 0; i < feature_list_all.size(); i++) {
        for (int j = 0; j < feature_list_all[i].size(); j++) {
//            cout<<feature_list_all[i][j].value <<"," << endl;
            w_file << feature_list_all[i][j].value<<",";
        }
        w_file<<endl;
    }

    cout <<"write done" << endl;
    waitKey(0);

    return 0;
}
#endif

// binary write
#if 00
int main(){
    int a[5] = {1,2,3,4,5};
    int b[5];

    ofstream ouF;
    ouF.open("./res/me.data", std::ofstream::binary);
    ouF.write(reinterpret_cast<const char*>(a), sizeof(int)*5);
    ouF.close();

    cout <<"write func test" << endl;
    return 0;
}
#endif

//text write
#if 00
int main(){
    int a[5] = {1,2,3,4,5};
    int b[5];


    ofstream file;

    file.open("./res/me2.data",ios::app);
    for(int i=0;i< 5;i++){
        file<<a[i] << ",";
    }
//    file<< "\n";
    file<<endl;
    file.close();

    cout <<"write func test" << endl;
    return 0;
}
#endif