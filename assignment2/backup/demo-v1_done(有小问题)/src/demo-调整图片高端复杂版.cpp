//
// Created by Tommy on 2020/5/13.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include<algorithm>
#include "decoding.h"
#include <math.h>

#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]


vector<Point2f>  find_points(Mat src){
    Mat mid_img,gray_img;
    medianBlur(src, mid_img, 5);

    cvtColor(mid_img, gray_img, COLOR_BGR2GRAY);
    vector<Vec3f> circles ;
    vector<Point2f> center_point;
    HoughCircles(gray_img , circles , HOUGH_GRADIENT, 1, 30, 100, 30, 1, 30);
    for( size_t i = 0; i < circles.size(); i++ ) {
        Vec3i c = circles[i];
        circle( src, Point(c[0], c[1]), c[2], Scalar(0,0,255) , 3, 1);
        circle(src, Point(c[0], c[1]), 2, Scalar(0,0,255));
        center_point.push_back(Point(c[0], c[1]));
    }
    return center_point;
}
int right_angle_points(vector<Point2f> points_3 ){
    Vec2f v0(points_3[0].x, points_3[0].y), v1(points_3[1].x, points_3[1].y),v2(points_3[2].x,points_3[2].y);

    Vec2f AA = v0-v1;
    Vec2f BB = v1-v2;
    Vec2f CC = v2-v0;

    float right_angle_flag;
    Vec2f right_angle;
    int right_angle_index;
    if(fabs(AA.dot(BB)) <fabs( BB.dot(CC)) ){
        right_angle_flag = abs(AA.dot(BB));
        right_angle = v1;
        right_angle_index =1;
    }
    else if( fabs(AA.dot(BB)) > fabs( BB.dot(CC)) ){
        right_angle_flag =  fabs( BB.dot(CC));
        right_angle = v2;
        right_angle_index =2;
    }
    else if( fabs( CC.dot(AA)) < right_angle_flag ){
        right_angle_flag = fabs( CC.dot(AA));
        right_angle = v0;
        right_angle_index =0;
    }

//    return right_angle;
    return right_angle_index;
}
#if 01
int main (int argc, char* argv[]){
    Mat emp_img = imread("./images/2Dempty.jpg",1);
//    Mat input_img = imread("./images/abcde_rotated.jpg",1);
    Mat input_img = imread("./images/congratulations_rotated_scaled.jpg",1);


    vector<Point2f> emp_circles = find_points(emp_img);
    vector<Point2f> input_circles = find_points(input_img);

    /*找到三个点位置*/
    Point2f srcTri[3] ={input_circles[0],input_circles[1],input_circles[2]};; //input
    Point2f dstTri[3] = {emp_circles[0],emp_circles[1],emp_circles[2]}; //empty

    line(emp_img, dstTri[0],dstTri[1], Scalar(0,0,255), 3);
    line(emp_img, dstTri[1],dstTri[2], Scalar(0,0,255), 3);
    line(emp_img, dstTri[2],dstTri[0], Scalar(0,0,255), 3);

    /*找直角索引*/ //  未摆正
    int empty_right_angle=  right_angle_points(emp_circles);
    cout << "wocao, empty right-angle point is : "<<empty_right_angle  << endl;
    int input_right_angle =  right_angle_points(input_circles);//
    cout << "wocao, input right-angle point is : "<<input_right_angle  << endl;

    /*两张图片的中心点*/
    int img_height = input_img.rows;
    int img_width = input_img.cols;
    int empty_img_height = emp_img.rows;
    int empty_img_width = emp_img.cols;

    /*testing  计算正弦值 */
    double k1 = (srcTri[input_right_angle].y - img_height/2.0) /(srcTri[input_right_angle].x - img_width/2.0) ;
    double k2 = (dstTri[empty_right_angle].y - empty_img_height/2.0) /(dstTri[empty_right_angle].x - empty_img_width/2.0) ; //??/
    double adjust_degree =atan(k1)*180/M_PI  - atan(k2)*180/M_PI;
    cout << "degree:"<<adjust_degree<<endl;

    Mat turn_up_img;
    cv::Point2f center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, adjust_degree, 1.0);
    cv::warpAffine(input_img, turn_up_img, rot_mat, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    imshow("input_img",input_img);
    imshow("temp",turn_up_img);

    /*计算直角边距离*/
    float dis;
    if(input_right_angle!=0){
        dis = powf((srcTri[input_right_angle].x - srcTri[0].x),2) + powf((srcTri[input_right_angle].y - srcTri[0].y),2);
    }else{
        dis = powf((srcTri[input_right_angle].x - srcTri[1].x),2) + powf((srcTri[input_right_angle].y - srcTri[1].y),2);
    }
    dis = sqrtf(dis);
    cout << "dis:"<<dis<<endl;

    /*调正后图片直角点坐标*/
    vector<Point2f> turn_up_circles = find_points(turn_up_img);
    int turn_up_right_angle=  right_angle_points(turn_up_circles);
    Point2f turn_up_Tri[3] = {turn_up_circles[0],turn_up_circles[1],turn_up_circles[2]}; //empty
    cout << "turn_up_right_angle:"<<turn_up_Tri[turn_up_right_angle]<<endl;

    /*旋转90度倍数*/
    int angle=0; //滚动角度
    if(turn_up_Tri[turn_up_right_angle].x < dis && turn_up_Tri[turn_up_right_angle].y<dis){
        cout << "正图"<<endl;
        angle = angle +0;
    }else if(turn_up_Tri[turn_up_right_angle].x < dis && turn_up_Tri[turn_up_right_angle].y < dis){
        cout << "转90"<<endl;
        angle = angle +90;
    }else if(turn_up_Tri[turn_up_right_angle].x > dis && turn_up_Tri[turn_up_right_angle].y <dis){
        cout << "转180"<<endl;
        angle = angle +180;
    }else if(turn_up_Tri[turn_up_right_angle].x > dis && turn_up_Tri[turn_up_right_angle].y > dis){
        cout << "转270"<<endl;
        angle = angle +270;
    }
    Mat dst_img;
    Mat M = getRotationMatrix2D(center,angle,1);//计算旋转的仿射变换矩阵
//    warpAffine(turn_up_img,dst_img,M,Size(turn_up_img.cols,turn_up_img.rows));//仿射变换
    cv::warpAffine(turn_up_img, dst_img, M, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    imshow("dst_img",dst_img);









    imshow("emp_img",emp_img);
//    imshow("input_img",input_img);
//    imshow("warp_dst",warp_dst);
//    imshow("input_adjust",input_adjust);
//    imshow("img_trans",img_trans);

//    imshow("Circles detected", gray_img);
    waitKey(0);
    return 1;

}
#endif
