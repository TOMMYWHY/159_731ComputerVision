

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<algorithm>
#include "decoding.h"
#include <math.h>

#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include <ImgModel.h>

using namespace std;
using namespace cv;

#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]

Mat first_rotate(Mat input_img);
Mat second_rotate(Mat input_img,vector<Point2f> input_3_point,float input_dis);
int convertBinaryToDecimal(char const* const binaryString);
string decode_bit_to_char(string bit_str);
#if 0000001
int main (int argc, char* argv[]){

    /*if (argc != 2) {printf("needs an input image\n");exit(0);}
    Mat input_img = imread ( argv [ 1 ] , 1) ;*/

    Mat emp_img = imread("./images/2Dempty.jpg",1);
//    imshow("emp_img",emp_img);

    ImgModel emp_model(emp_img);

//    Mat input_img = imread("./images/abcde.jpg",1);
//    Mat input_img = imread("./images/abcde_rotated.jpg",1);
//    Mat input_img = imread("./images/abcde_scaled.jpg",1);
//    Mat input_img = imread("./images/abcde_rotated_scaled.jpg",1);

//    Mat input_img = imread("./images/congratulations.jpg",1);
//    Mat input_img = imread("./images/congratulations_rotated.jpg",1);
//    Mat input_img = imread("./images/congratulations_rotated_scaled.jpg",1);
//    Mat input_img = imread("./images/congratulations_scaled.jpg",1);
//
//    Mat input_img = imread("./images/Darwin.jpg",1);
//    Mat input_img = imread("./images/Darwin_rotated.jpg",1);//
//    Mat input_img = imread("./images/Darwin_scaled.jpg",1);
    Mat input_img = imread("./images/Darwin_rotated_scaled.jpg",1);

//        Mat input_img = imread("./images/farfaraway.jpg",1);
//    Mat input_img = imread("./images/farfaraway_rotated.jpg",1);
//    Mat input_img = imread("./images/farfaraway_scaled.jpg",1);
//    Mat input_img = imread("./images/farfaraway_rotated_scaled.jpg",1); //18

    imshow("input_img",input_img);

    /*first rotate */
    Mat input_img_first_rotated =  first_rotate(input_img);
    imshow("input_img_first_rotated",input_img_first_rotated);

    ImgModel input_model(input_img_first_rotated);
//    cout <<"-----input_model------:" <<endl;
//    input_model.to_string();
//    cout <<"---------" <<endl;

    float resize_ratio = 1/(input_model.distance/emp_model.distance); //todo

    Mat resize_input;
//    cout <<"resize_ratio:"<< resize_ratio <<endl;
    resize(input_model.Img,resize_input,
            Size(round(input_model.Img.cols*resize_ratio),round(input_model.Img.rows*resize_ratio)),
            0,0,INTER_LINEAR);

//    input_model.Img = resize_input;
//    cout <<"******resize****  input_model:" <<endl;
//    input_model.to_string();
//    cout <<"---------" <<endl;
    imshow("resize_ratio_input",resize_input);

    /* second rotate */
    Mat dst_img =  second_rotate(resize_input,input_model.three_circles_points,input_model.distance);
//    imshow("dst_img",dst_img);

    ImgModel dst_model(dst_img);
//    cout <<"******dst_model:" <<endl;
//    dst_model.to_string();
//    cout <<"---------" <<endl;

    int circle_occupy_top_w = emp_model.start_p.x + emp_model.sub_box_w*6;
    int circle_occupy_bottom_w = emp_model.start_p.x + emp_model.distance - emp_model.sub_box_w ;

    Mat middle ;
    middle =emp_img.clone();

    for (int y = emp_model.start_p.y; y < emp_model.end_p.y; y=y+20) {
        for (int x = emp_model.start_p.x; x < emp_model.end_p.x; x=x+20) {
            Point c;
            c.x = x+ emp_model.sub_box_w/2;
            c.y = y+ emp_model.sub_box_w/2;
            if(x< circle_occupy_top_w && y < circle_occupy_top_w - emp_model.sub_box_w){
                circle(middle,c, 3, Scalar(0,255,0),3);//green
            } else if (x < circle_occupy_top_w  && y > circle_occupy_bottom_w  ){
                circle(middle,c, 3, Scalar(255,0,0),3);//blue
            }else if(x > circle_occupy_bottom_w  && y >circle_occupy_bottom_w ){
                circle(middle,c, 3, Scalar(0,255,255),3);//yellow
            }else{
                circle(middle,c, 3, Scalar(0,0,255));
                PointInfo p;
                p.x = c.x;
                p.y = c.y;
                p.b = int(MpixelB(emp_img ,x,y));
                p.g = int(MpixelG(emp_img ,x,y));
                p.r = int(MpixelR(emp_img ,x,y));
                emp_model.sub_boxes.push_back(p);
            }
        }
    }
    imshow("middle",middle);

//    cout <<"emp_model:" <<endl;
//    emp_model.to_string();
//    cout <<"---------" <<endl;
//    dst_model.to_string();

    for (int i = 0; i < emp_model.sub_boxes.size(); i++)
    {
        int padding_x,padding_y;
        padding_x = (dst_model.start_p.x - emp_model.start_p.x);
        padding_y = (dst_model.start_p.y - emp_model.start_p.y);
//        cout << "x:"<<emp_model.sub_boxes[i].x<<"y:"<< emp_model.sub_boxes[i].y <<endl;
        int x = emp_model.sub_boxes[i].x + padding_x;
        int y = emp_model.sub_boxes[i].y + padding_y;
//        cout << "new x:"<<x<<"new y:"<< y <<endl;
        PointInfo p;
        p.x = x;
        p.y = y;
        p.b = int(MpixelB(dst_img ,x,y));
        p.g = int(MpixelG(dst_img ,x,y));
        p.r = int(MpixelR(dst_img ,x,y));
        p.b=p.b>128 ? 1:0;
        p.g=p.g>128 ? 1:0;
        p.r=p.r>128 ? 1:0;
        dst_model.sub_boxes.push_back(p);
    }

    string bit_str ;
    for (int i = 0; i < dst_model.sub_boxes.size()-1; i++) {
        Point c;
        c.x =dst_model.sub_boxes[i].x;
        c.y =dst_model.sub_boxes[i].y;
        circle(dst_img,c, 4, Scalar(0,0,255),1);//yellow
       /* cout <<" x: " << dst_model.sub_boxes[i].x
             <<" y: " << dst_model.sub_boxes[i].y
             <<" b: " << dst_model.sub_boxes[i].b
             <<" g: " << dst_model.sub_boxes[i].g
             <<" r: " << dst_model.sub_boxes[i].r
        <<endl;*/

        string s_b=to_string(dst_model.sub_boxes[i].b);
        string s_g=to_string(dst_model.sub_boxes[i].g);
        string s_r=to_string(dst_model.sub_boxes[i].r);
        string res = s_r+s_g+s_b;
        bit_str +=res;
    }
    imshow("dst_img",dst_img);
    cout <<"decode_bit_str:" <<bit_str<<endl;

    string decode_char = decode_bit_to_char(bit_str);
    cout << "====="<< endl;
    cout << "====="<< endl;
    cout << "===== decode result : "<< endl;
    cout << decode_char<< endl;

    waitKey(0);
    return 1;

}
#endif

Mat first_rotate(Mat input_img){
    Mat  binary_img,adjust_img;
    int th1 = 50,th2=150,maxLineGap=10;

    Canny(input_img, binary_img, 50,100,3);

    vector<Vec4i> plines;
    HoughLinesP(binary_img, plines, 1, CV_PI/180,th1,th2,maxLineGap);
    Scalar color = Scalar(0, 0, 255);

    input_img.copyTo(adjust_img);

    double degree;
    float k =(float) ( plines[0][3]- plines[0][1]) /( plines[0][2]- plines[0][0]);
    degree =atan(k)*180/M_PI;
//    cout << "fun::first_rotate => first rotate degree: " << degree<<endl;

    cv::Point2f center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, degree, 1.0);
    cv::warpAffine(input_img, adjust_img, rot_mat, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    return adjust_img;
}
Mat second_rotate(Mat input_img,vector<Point2f> input_3_point,float input_dis){
    int angle=0;
    if(input_3_point[1].x<input_dis && input_3_point[1].y > input_dis){
//        cout << "same with empty_img"<<endl;// point[1] at left bottom.  x should smaller than distance，y should bigger than distance
    }else if(input_3_point[1].x<input_dis && input_3_point[1].y < input_dis){
//        cout << "need rotate 90"<<endl;
        angle=90;
    }else if(input_3_point[1].x>input_dis && input_3_point[1].y<input_dis){
//        cout << "need rotate 180"<<endl;
        angle=180;
    }else if(input_3_point[1].x>input_dis && input_3_point[1].y>input_dis){
//        cout << "need rotate 270"<<endl;
        angle=270;
    }

    // todo resize
    cv::Point2f center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    Mat dst_img;
    Mat M = getRotationMatrix2D(center,angle,1);
    warpAffine(input_img ,dst_img,M,Size(input_img.cols,input_img.rows));
    return dst_img;
}
int convertBinaryToDecimal(char const* const binaryString)
{
    //在此处调用判断一个字符串有多长的函数
    int size = strlen(binaryString);
    //将二进制数字转换为十进制
    int parseBinary = 0;
    for (int i = size; i >= 0; --i) {
        if (binaryString[i] == '1') {
            parseBinary += pow(2.0, i);
        }
    }
    return parseBinary;
}
string decode_bit_to_char(string bit_str){
    string decode_to_char;

    for (int i = 0; i < bit_str.size(); i=i+6) {
        if (i%6==0){
            string sigle = bit_str.substr(i, 6);
            reverse(sigle.begin(),sigle.end());
            const char* arr = sigle.data();
            int ret = convertBinaryToDecimal(arr);
            cout << sigle<<"=>"<< ret <<"  ;  ";
            decode_to_char +=encodingarray[ret];
        }
    }
    return decode_to_char;
}
