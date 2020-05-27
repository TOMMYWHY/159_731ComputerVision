//
// Created by Tommy on 2020/5/13.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<algorithm>
#include "decoding.h"
#include <math.h>

#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;
using namespace cv;

#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]


vector<Point2f>  find_points(Mat src ){
    Mat mid_img,gray_img;
    medianBlur(src, mid_img, 5);

    cvtColor(mid_img, gray_img, COLOR_BGR2GRAY);
    vector<Vec3f> circles ;
    vector<Point2f> center_point;
    HoughCircles(gray_img , circles , HOUGH_GRADIENT, 1, 50, 100, 50, 30, 50);
    for( size_t i = 0; i < circles.size(); i++ ) {
        Vec3i c = circles[i];
        Scalar scalar = Scalar(0,0,255);
//        circle( src, Point(c[0], c[1]), c[2], scalar , 3, 1);
        circle(src, Point(c[0], c[1]), 2, scalar);
        center_point.push_back(Point(c[0], c[1]));
    }
    return center_point;
}
int right_angle_points(vector<Point2f> points_3 ){
//    cout << " points_3: "<<points_3  << endl;
    Vec2f v0(points_3[0].x, -points_3[0].y), v1(points_3[1].x, -points_3[1].y),v2(points_3[2].x,-points_3[2].y);

    Vec2f aa = v0-v1;
    Vec2f bb = v1-v2;
    Vec2f cc = v2-v0;

    Vec2f right_angle;
    int right_angle_index;
//    cout <<"fun:right_angle_points ->aa.dot(bb)" <<abs(aa.dot(bb))<<endl;
//    cout <<"fun:right_angle_points ->bb.dot(cc)" <<abs(bb.dot(cc))<<endl;
//    cout <<"fun:right_angle_points ->cc.dot(aa)" <<abs( cc.dot(aa))<<endl;

    float min = abs(aa.dot(bb));
    right_angle_index =1;
    if(abs( bb.dot(cc)) < min){
        min =abs( bb.dot(cc));
        right_angle_index =2;
    }
    if(abs( cc.dot(aa))< min){
        min =abs( cc.dot(aa));
        right_angle_index =0;
    }
    return right_angle_index;
}
Mat fine_tuning(Mat input_img){
    Mat input_binary_img , input_alignment;
    int th1 = 50,th2=150,maxLineGap=10;
    Canny(input_img, input_binary_img, 50,100,3);

    vector<Vec4i> plines;
    HoughLinesP(input_binary_img, plines, 1, CV_PI/180,th1,th2,maxLineGap);
    Scalar color = Scalar(0, 0, 255);

    input_img.copyTo(input_alignment);

    double degree_alignment;
    float k =(float) ( plines[0][3]- plines[0][1]) /( plines[0][2]- plines[0][0]);
    degree_alignment =atan(k)*180/M_PI;
//    cout <<"fine tuning angle degree (first rotate) : " <<degree_alignment <<endl;
    cv::Point2f input_center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_alignment = cv::getRotationMatrix2D(input_center, degree_alignment, 1.0);
    cv::warpAffine(input_img, input_alignment, rot_alignment, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    return input_alignment;
}


int convertBinaryToDecimal(char const* const binaryString)
{
    int size = strlen(binaryString);
    int parseBinary = 0;
    for (int i = size; i >= 0; --i) {
        if (binaryString[i] == '1') {
            parseBinary += pow(2.0, i);
        }
    }
    return parseBinary;
}

vector<Point2f> reposition_right_angle(vector<Point2f> turn_up_circles ,int right_angle_index){
    Point2f turn_up_Tri[3] = {turn_up_circles[0],turn_up_circles[1],turn_up_circles[2]};
    vector<Point2f> turn_up_Points_vct = {turn_up_circles[0],turn_up_circles[1],turn_up_circles[2]}; // temp
    turn_up_Points_vct.erase(turn_up_Points_vct.begin()+right_angle_index); //
    turn_up_Points_vct.insert(turn_up_Points_vct.begin()+1, turn_up_Tri[right_angle_index]);
    return turn_up_Points_vct;
}


#if 000001
int main (int argc, char* argv[]){

    /*if (argc != 2) {printf("needs an input image\n");exit(0);}
    Mat input_img = imread ( argv [ 1 ] , 1) ;*/

    Mat emp_img = imread("./images/2Dempty.jpg",1);

//    Mat input_img = imread("./images/abcde.jpg",1);
//    Mat input_img = imread("./images/abcde_rotated.jpg",1);
//    Mat input_img = imread("./images/abcde_scaled.jpg",1);//
//    Mat input_img = imread("./images/abcde_rotated_scaled.jpg",1);// 16  aa

//    Mat input_img = imread("./images/congratulations.jpg",1);
//    Mat input_img = imread("./images/congratulations_rotated.jpg",1);
//    Mat input_img = imread("./images/congratulations_rotated_scaled.jpg",1);
//    Mat input_img = imread("./images/congratulations_scaled.jpg",1); //  24
//
//    Mat input_img = imread("./images/Darwin.jpg",1);// 20
//    Mat input_img = imread("./images/Darwin_rotated.jpg",1);//
//    Mat input_img = imread("./images/Darwin_scaled.jpg",1);//  22 902
//    Mat input_img = imread("./images/Darwin_rotated_scaled.jpg",1);  //  22 894

//        Mat input_img = imread("./images/farfaraway.jpg",1); //20
//    Mat input_img = imread("./images/farfaraway_rotated.jpg",1); //
//    Mat input_img = imread("./images/farfaraway_scaled.jpg",1); //18
    Mat input_img = imread("./images/farfaraway_rotated_scaled.jpg",1); //18

//    imshow("emp_img",emp_img);
    imshow("input_img",input_img);

    vector<Point2f> temp_input_circles = find_points(input_img);
    imshow("input_img",input_img);

    Mat input_alignment = fine_tuning(input_img);
    imshow("input_alignment",input_alignment);

    vector<Point2f> emp_circles = find_points(emp_img);
    Point2f dstTri[3] = {emp_circles[0],emp_circles[1],emp_circles[2]}; //empty
    imshow("emp_img",emp_img);
    //[92, 90; 90, 910; 910, 910]

//    int empty_right_angle=  right_angle_points(emp_circles);

    vector<Point2f> turn_up_circles = find_points(input_alignment);
    if(turn_up_circles[0].x==0 && turn_up_circles[1].x==0 && turn_up_circles[2].x==0){
        return -1;
    }
    int turn_up_right_angle =  right_angle_points(turn_up_circles);

    vector<Point2f>  turn_up_Points_vct = reposition_right_angle(turn_up_circles,turn_up_right_angle);


//    double emp_dis = sqrt(powf((emp_circles[1].x - emp_circles[0].x),2) + powf((emp_circles[1].y - emp_circles[0].y),2));
    double input_dis = sqrt(powf((turn_up_Points_vct[1].x - turn_up_Points_vct[0].x),2) + powf((turn_up_Points_vct[1].y - turn_up_Points_vct[0].y),2));
    int angle=0;
    if(turn_up_Points_vct[1].x<input_dis && turn_up_Points_vct[1].y > input_dis){
    }else if(turn_up_Points_vct[1].x<input_dis && turn_up_Points_vct[1].y < input_dis){
        angle=90;
    }else if(turn_up_Points_vct[1].x>input_dis && turn_up_Points_vct[1].y<input_dis){
        angle=180;
    }else if(turn_up_Points_vct[1].x>input_dis && turn_up_Points_vct[1].y>input_dis){
        angle=270;
    }

    cv::Point2f center(static_cast<float>(input_alignment.cols / 2.), static_cast<float>(input_alignment.rows / 2.));
    Mat dst_img;
    Mat M = getRotationMatrix2D(center,angle,1);//
    warpAffine(input_alignment ,dst_img,M,Size(input_alignment.cols,input_alignment.rows));

    vector<Point2f> dst_circles = find_points(dst_img);
    int dis_right_angle_index=  right_angle_points(dst_circles);
    vector<Point2f>  dis_Points_vct = reposition_right_angle(dst_circles,dis_right_angle_index);
    double dst_dis = sqrt(powf((dis_Points_vct[1].x - dis_Points_vct[0].x),2) + powf((dis_Points_vct[1].y - dis_Points_vct[0].y),2));
    Mat input_threshold = dst_img;//
//===============================

    int circle_occupy_sqt = 6; //
    /* 47*47 */
    int each_w =int( dst_dis /(47-circle_occupy_sqt));//dis = l - 2*r;  2*r = circle_occupy_sqt
    Point scan_begin,scan_end;

    scan_begin.x =dis_Points_vct[1].x-3*each_w; // r = 3*each_w
    scan_begin.y =dis_Points_vct[1].y - dst_dis-3*each_w;
    scan_end.x=scan_begin.x+dst_dis+6*each_w;
    scan_end.y=scan_begin.y+dst_dis+6*each_w;

    cout << "each_w: " << each_w<< endl;
    int nth_s = scan_begin.x + each_w*circle_occupy_sqt; //nothing there short
    int nth_l =scan_begin.x + int(dst_dis)  ; //nothing there long
    Mat test;//
    vector<Point> std_points ;//
    test.create(input_threshold.size(),CV_8UC3);

    string decode_img ;
    int get_point_count = 0;

    for (int y=scan_begin.y;y<scan_end.y; y++)
    {
        for ( int x=scan_begin.x;x<scan_end.x ;  x++)
        {
            Vec3b pixel_image = input_threshold.at<Vec3b>(y, x);
            Point c;
            c.x = x;
            c.y = y;
            if(x< nth_s && y < nth_s){
                circle(input_threshold,c, 1, Scalar(0,0,255),1);//green

            } else if (x < nth_s  && y > nth_l  ){
                circle(input_threshold,c, 1, Scalar(0,0,255),1);//blue

            }else if(x > nth_l  && y >nth_l ){
                circle(input_threshold,c, 1, Scalar(0,0,255),1);//yellow
            } else{
                Point c;
                c.x = x+ each_w/3;
                c.y = y+ each_w/3;
                if( x%each_w==(each_w/2) && y%each_w==each_w/2)
                {
                    circle(input_threshold,c, 2, Scalar(0,0,255),1);//yellow
                    get_point_count ++;
                    test.at<Vec3b>(y,x)=pixel_image;
                    int b = int(MpixelB(input_threshold ,c.x,c.y));
                    int g = int(MpixelG(input_threshold ,c.x,c.y));
                    int r = int(MpixelR(input_threshold ,c.x,c.y));

                    b=b>128 ? 1:0;
                    g=g>128 ? 1:0;
                    r=r>128 ? 1:0;
                    string s_b=to_string(b);
                    string s_g=to_string(g);
                    string s_r=to_string(r);
                    string res = s_r+s_g+s_b;
                    decode_img += res;
                }
            }
            test.at<Vec3b>(y,x)=pixel_image;
        }
    }
    cout <<"get_point_count:" <<get_point_count<<endl;

    imshow("test",test);

    string decode_to_char;
    for (int i = 0; i < (get_point_count -1)*3; i=i+6) {
        if (i%6==0){
            string sigle = decode_img.substr(i, 6);
            reverse(sigle.begin(),sigle.end());
            const char* arr = sigle.data();
            int ret = convertBinaryToDecimal(arr);
            decode_to_char +=encodingarray[ret];
        }
    }
    cout << decode_to_char<< endl;

    waitKey(0);
    return 1;

}
#endif


