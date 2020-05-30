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

//    cout <<emp_circles[0] <<endl;
//    line(emp_img, emp_circles[0],emp_circles[1], Scalar(0,0,255), 30);


    Point2f srcTri[3] ={input_circles[0],input_circles[1],input_circles[2]};; //input

    Point2f dstTri[3] = {emp_circles[0],emp_circles[1],emp_circles[2]}; //empty


    Vec2f v0(input_circles[0].x, input_circles[0].y), v1(input_circles[1].x, input_circles[1].y),v2(input_circles[2].x,input_circles[2].y);

    Vec2f AA = v0-v1;
    Vec2f BB = v1-v2;
    Vec2f CC = v2-v0;

    float right_angle_flag;
    Vec2f right_angle;
    if(fabs(AA.dot(BB)) <fabs( BB.dot(CC)) ){
        right_angle_flag = abs(AA.dot(BB));
        right_angle = v1;
    }
    else if( fabs(AA.dot(BB)) > fabs( BB.dot(CC)) ){
        right_angle_flag =  fabs( BB.dot(CC));
        right_angle = v2;
    }
    else if( fabs( CC.dot(AA)) < right_angle_flag ){
        right_angle_flag = fabs( CC.dot(AA));
        right_angle = v0;
    }

//    return right_angle;
    cout <<right_angle <<endl;

/*
    double degree;
    if(right_angle!=v1){
        float k =(float) ( right_angle[1]- v1[1]) /( right_angle[0]- v1[0]);
        degree =atan(k)*180/M_PI;
    }else if(right_angle!=v2){
        float k =(float) ( right_angle[1]- v2[1]) /( right_angle[0]- v2[0]);
        degree =atan(k)*180/M_PI;
    }else{
        float k =(float) ( right_angle[1]- v0[1]) /( right_angle[0]- v0[0]);
        degree =atan(k)*180/M_PI;
    }

    cout << degree <<endl;
    Mat input_adjust;
    Point2f input_center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    Mat rot_mat = cv::getRotationMatrix2D(input_center, degree, 1.0);
    warpAffine(input_img, input_adjust, rot_mat, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
*/




//    float k =(float) ( right_angle[0]- plines[0][1]) /( right_angle[1]- plines[0][0]);
//    degree =atan(k)*180/M_PI;


/////

    int empty_right_angle=  right_angle_points(emp_circles);
    cout << "wocao, empty right-angle point is : "<<empty_right_angle  << endl;

//    /*vector<Point2f> input_adjust_circles = find_points(input_adjust);

    int input_right_angle =  right_angle_points(input_circles);// todo 未摆正
    cout << "wocao, input right-angle point is : "<<input_right_angle  << endl;

    int img_height = input_img.rows;
    int img_width = input_img.cols;
    int empty_img_height = emp_img.rows;
    int empty_img_width = emp_img.cols;

    /*testing  计算正弦值 */
    double k1 = (srcTri[input_right_angle].y - img_height/2.0) /(srcTri[input_right_angle].x - img_width/2.0) ;
    double k2 = (dstTri[empty_right_angle].y - empty_img_height/2.0) /(dstTri[empty_right_angle].x - empty_img_height/2.0) ; //??/
//    cout << "k2 dian:"<<dstTri[empty_right_angle]<<endl;
//    cout << "k2 dian:"<<(dstTri[empty_right_angle].y - empty_img_height/2.0)  << ";"<<(dstTri[empty_right_angle].x - empty_img_height/2.0) <<endl;

    cout << "k1:"<<k1<<endl;
    cout << "k2:"<<k2<<endl;

    double degree =atan(k1)*180/M_PI  - atan(k2)*180/M_PI;
    cout << "degree:"<<degree<<endl;
    float dis;
    if(input_right_angle!=0){
        dis = powf((srcTri[input_right_angle].x - srcTri[0].x),2) + powf((srcTri[input_right_angle].y - srcTri[0].y),2);
    }else{
        dis = powf((srcTri[input_right_angle].x - srcTri[1].x),2) + powf((srcTri[input_right_angle].y - srcTri[1].y),2);
    }
    dis = sqrtf(dis);
    cout << "dis:"<<dis<<endl;
    cout << "input_right_angle:"<<srcTri[input_right_angle]<<endl;


//    degree = degree +90+90;
    if(srcTri[input_right_angle].x + dis == srcTri[input_right_angle].y){
        cout << "正图"<<endl;
        degree = degree +0;
    }else if(srcTri[input_right_angle].x < dis && srcTri[input_right_angle].y < dis){
        cout << "转90"<<endl;
        degree = degree +90;
    }else if(srcTri[input_right_angle].x > dis && srcTri[input_right_angle].y <dis){
        cout << "转180"<<endl;
        degree = degree +180;
    }else if(srcTri[input_right_angle].x > dis && srcTri[input_right_angle].y > dis){
        cout << "转270"<<endl;
        degree = degree +270;
    }

    Mat temp;
    cv::Point2f center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, degree, 1.0);
    cv::warpAffine(input_img, temp, rot_mat, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    imshow("input_img",input_img);

    imshow("temp",temp);
    /*  int rotate_angle=0;
      if(){
          cout << "正图"<<endl;
      } else if (){
          cout << "转90"<<endl;
          rotate_angle=90;
      }else if (){
          cout << "转180"<<endl;
          rotate_angle=180;
      }else if (){
          cout << "转270"<<endl;
          rotate_angle=270;
      }*/




    /* Vec2f v0(emp_circles[0].x, emp_circles[0].y), v1(emp_circles[1].x, emp_circles[1].y),v2(emp_circles[2].x,emp_circles[2].y);

     Vec2f AA = v0-v1;
     Vec2f BB = v1-v2;
     Vec2f CC = v2-v0;

     float right_angle_flag;
     Vec2f right_angle;
     if(fabs(AA.dot(BB)) <fabs( BB.dot(CC)) ){
         right_angle_flag = abs(AA.dot(BB));
         right_angle = v1;
     }
     else if( fabs(AA.dot(BB)) > fabs( BB.dot(CC)) ){
         right_angle_flag =  fabs( BB.dot(CC));
         right_angle = v2;
     }
     else if( fabs( CC.dot(AA)) < right_angle_flag ){
         right_angle_flag = fabs( CC.dot(AA));
         right_angle = v0;
     }

     cout << "wocao, right-angle point is : "<<right_angle  << endl;*/

    line(emp_img, dstTri[0],dstTri[1], Scalar(0,0,255), 3);
    line(emp_img, dstTri[1],dstTri[2], Scalar(0,0,255), 3);
    line(emp_img, dstTri[2],dstTri[0], Scalar(0,0,255), 3);

    Mat src, warp_dst, warp_rotate_dst;

    warp_dst = Mat::zeros( emp_img.rows, emp_img.cols, emp_img.type() );

    Mat warp_mat = getAffineTransform( srcTri, dstTri  );
//    Mat warp_mat = getAffineTransform( srcTri, srcTri_repos  );
//    Mat warp_mat = getPerspectiveTransform( srcTri, dstTri  ); //可用vector
//    cout<<warp_mat<<endl;
//    int img_height = input_img.rows;
//    int img_width = input_img.cols;

//    perspectiveTransform( ponits, points_trans, warp_mat);
    Mat img_trans = Mat::zeros(img_height,img_width,CV_8UC3);


//    perspectiveTransform( ponits, points_trans, transform);

    warpAffine( input_img, img_trans, warp_mat, warp_dst.size() );





    /*Mat gray_img, mid_img ;
    medianBlur(emp_img, mid_img, 5);
    cvtColor(mid_img, gray_img, COLOR_BGR2GRAY);
    vector<Vec3f> circles ;
    HoughCircles(gray_img , circles , HOUGH_GRADIENT, 1, 10, 100, 30, 1, 30);
    for( size_t i = 0; i < circles.size(); i++ ) {
        Vec3i c = circles[i];
        circle( gray_img, Point(c[0], c[1]), c[2], Scalar(0,0,255) , 3, 1);
        circle( emp_img, Point(c[0], c[1]), c[2], Scalar(0,0,255) , 3, 1);
        circle(emp_img, Point(c[0], c[1]), 1, Scalar(0,0,255));
    }*/

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
