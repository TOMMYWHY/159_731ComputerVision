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

struct PointInfo{
    int x;
    int y;
    unsigned int value;
};
struct ImgInfo{
    Point start_p;
    Point end_p;
    vector<Point2f> corrcet_right_angle_points;
    int padding_x; //start_p
    int padding_y;
    float distance;
    int sub_box_w;
    vector<PointInfo> sub_boxes;
};

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
    cout << "fun::first_rotate => first rotate degree: " << degree<<endl;

    cv::Point2f center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, degree, 1.0);
    cv::warpAffine(input_img, adjust_img, rot_mat, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    return adjust_img;
}
float calculate_distance(vector<Point2f>  input_3_point){
    float dis;
    float temp_dis[3];
    temp_dis[0] = sqrt(powf((input_3_point[1].x - input_3_point[0].x),2) + powf((input_3_point[1].y - input_3_point[0].y),2));
    temp_dis[1] = sqrt(powf((input_3_point[2].x - input_3_point[1].x),2) + powf((input_3_point[2].y - input_3_point[1].y),2));
    temp_dis[2] = sqrt(powf((input_3_point[0].x - input_3_point[2].x),2) + powf((input_3_point[0].y - input_3_point[2].y),2));
    dis = temp_dis[0];
    for (int i = 0; i < 3; i++) {
//        cout  <<temp_dis[i]<< endl;
        if(dis > temp_dis[i]){
            dis = temp_dis[i];
        }
    }
    cout<<"calculate_distance:: => dis:" << dis<<endl;
    return dis;
}
Mat second_rotate(Mat input_img,vector<Point2f> input_3_point,float input_dis,float resize){
    int angle=0;
    if(input_3_point[1].x<input_dis && input_3_point[1].y > input_dis){
        cout << "正图"<<endl;// 一号位在左下 x 小，y 大
    }else if(input_3_point[1].x<input_dis && input_3_point[1].y < input_dis){
        cout << "转90"<<endl;
        angle=90;
    }else if(input_3_point[1].x>input_dis && input_3_point[1].y<input_dis){
        cout << "转180"<<endl;
        angle=180;
    }else if(input_3_point[1].x>input_dis && input_3_point[1].y>input_dis){
        cout << "转270"<<endl;
        angle=270;
    }

    cv::Point2f center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    Mat dst_img;
    Mat M = getRotationMatrix2D(center,angle,1);//计算旋转的仿射变换矩阵
    warpAffine(input_img ,dst_img,M,Size(round(resize*input_img.cols),round(resize*input_img.rows)));//仿射变换
    return dst_img;
}


vector<Point2f>  find_points(Mat src ,  Scalar scalar){
    Mat mid_img,gray_img;
    medianBlur(src, mid_img, 5);

    cvtColor(mid_img, gray_img, COLOR_BGR2GRAY);
    vector<Vec3f> circles ;
    vector<Point2f> center_point;
    HoughCircles(gray_img , circles , HOUGH_GRADIENT, 1, 50, 100, 50, 30, 50);
    for( size_t i = 0; i < circles.size(); i++ ) {
        Vec3i c = circles[i]; //todo v2
//        circle( src, Point(c[0], c[1]), c[2], scalar , 3, 1);
        circle(src, Point(c[0], c[1]), 2, scalar);
        center_point.push_back(Point(c[0], c[1]));
    }
    return center_point;
}
int right_angle_points(vector<Point2f> points_3 ){
//    cout << " points_3: "<<points_3  << endl;
    Vec2f v0(points_3[0].x, -points_3[0].y), v1(points_3[1].x, -points_3[1].y),v2(points_3[2].x,-points_3[2].y);

    Vec2f AA = v0-v1;
    Vec2f BB = v1-v2;
    Vec2f CC = v2-v0;

    Vec2f right_angle;
    int right_angle_index;
   /* cout <<"fun:right_angle_points ->AA.dot(BB)" <<abs(AA.dot(BB))<<endl;
    cout <<"fun:right_angle_points ->BB.dot(CC)" <<abs(BB.dot(CC))<<endl;
    cout <<"fun:right_angle_points ->CC.dot(AA)" <<abs( CC.dot(AA))<<endl;*/

    float min = abs(AA.dot(BB));
    right_angle_index =1;
    if(abs( BB.dot(CC)) < min){
        min =abs( BB.dot(CC));
        right_angle_index =2;
    }
    if(abs( CC.dot(AA))< min){
        min =abs( CC.dot(AA));
        right_angle_index =0;
    }
    return right_angle_index;
}


void reposition_right_angle(vector<Point2f> &input_3_point ,int right_angle_index){
    Point2f temp = input_3_point[right_angle_index];
//    cout<<"temp:"<<temp <<endl;
    input_3_point.erase(input_3_point.begin()+right_angle_index);
    input_3_point.insert(input_3_point.begin()+1,temp);

//    cout <<"&&&&&&&&&" <<endl;
//    cout <<turn_up_circles <<endl;
}


void points_swap_to_right_pos(vector<Point2f> &input_3_point){
    /*to be 0,1,2 */
//    0
//    |\
//    | \
//    |  \
//    1---2
    if(input_3_point[0].x > input_3_point[2].x){
        swap(input_3_point[0],input_3_point[2]);
    }
};
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


#if 000001
int main (int argc, char* argv[]){

    /*if (argc != 2) {printf("needs an input image\n");exit(0);}
    Mat input_img = imread ( argv [ 1 ] , 1) ;*/

    Mat emp_img = imread("./images/2Dempty.jpg",1);
    imshow("emp_img",emp_img);

//    Mat input_img = imread("./images/abcde.jpg",1);
//    Mat input_img = imread("./images/abcde_rotated.jpg",1);
//    Mat input_img = imread("./images/abcde_scaled.jpg",1);//
//    Mat input_img = imread("./images/abcde_rotated_scaled.jpg",1);// 16

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
    Mat input_img = imread("./images/farfaraway_rotated.jpg",1); // todo 19
//    Mat input_img = imread("./images/farfaraway_scaled.jpg",1); //18
//    Mat input_img = imread("./images/farfaraway_rotated_scaled.jpg",1); //18

//    imshow("emp_img",emp_img);
    /*empty iamge*/
    vector<Point2f> empty_3_point = find_points(emp_img,Scalar(255,255,0));
    cout <<"empty_3_point:" <<empty_3_point<<endl;

    imshow("input_img",input_img);
    //first rotated
    Mat input_img_first_rotated =  first_rotate(input_img);
    imshow("input_img_first_rotated",input_img_first_rotated);

    vector<Point2f> input_3_point = find_points(input_img_first_rotated,Scalar(255,255,0));
    cout <<"input_3_point:" <<input_3_point<<endl;



    int enp_right_angle_index =  right_angle_points(empty_3_point);
//    cout <<"enp_right_angle_index:" <<enp_right_angle_index<<endl;
//    cout << "************************"<<endl;
//    cout <<"empty_3_point:" <<empty_3_point<<endl;
    reposition_right_angle(empty_3_point,enp_right_angle_index);
    cout <<" after reposition_right_angle  empty_3_point:" <<empty_3_point<<endl; //
    points_swap_to_right_pos(empty_3_point);
    cout <<" after points_swap_to_right_pos  empty_3_point:" <<empty_3_point<<endl; //

    cout << "************************"<<endl;

    int input_right_angle_index =  right_angle_points(input_3_point);
    cout <<"input_right_angle_index:" <<input_right_angle_index<<endl;
    if(input_right_angle_index != 1){
     reposition_right_angle(input_3_point,input_right_angle_index);
    }
    cout <<" after reposition_right_angle  input_3_point:" <<input_3_point<<endl; //
    float emp_dis = calculate_distance(empty_3_point);
    float input_dis = calculate_distance(input_3_point);
    float resize = 1/(input_dis/emp_dis);
    Mat dst_img =  second_rotate(input_img_first_rotated,input_3_point,input_dis,resize);
    imshow("dst_img",dst_img);



    //===============================
    ImgInfo empty_info;
    Point start_p_by_index1,start_p_by_index0;
    empty_info.distance = emp_dis;
    empty_info.sub_box_w =int( empty_info.distance /(47-6));

    empty_info.corrcet_right_angle_points = empty_3_point;
    start_p_by_index1.x = empty_info.corrcet_right_angle_points[1].x - 3 * empty_info.sub_box_w;
    start_p_by_index1.y = empty_info.corrcet_right_angle_points[1].y - empty_info.distance - 3 * empty_info.sub_box_w;

    start_p_by_index0.x = empty_info.corrcet_right_angle_points[0].x -3*empty_info.sub_box_w;
    start_p_by_index0.y = empty_info.corrcet_right_angle_points[0].y -3*empty_info.sub_box_w;

    empty_info.start_p.x  = int((start_p_by_index0.x+start_p_by_index1.x)/2);
    empty_info.start_p.y  = int((start_p_by_index0.y+start_p_by_index1.y)/2);
    cout <<"x:"  << empty_info.start_p.x << " y:"<< empty_info.start_p.y <<endl;
    empty_info.end_p.x = empty_info.start_p.x + empty_info.distance + 6 *empty_info.sub_box_w;
    empty_info.end_p.y = empty_info.start_p.y + empty_info.distance + 6 *empty_info.sub_box_w;
    cout <<"x:"  << empty_info.end_p.x << " y:"<< empty_info.end_p.y <<endl;

    int circle_occupy_top_w = empty_info.start_p.x + empty_info.sub_box_w*6;
    int circle_occupy_bottom_w = empty_info.start_p.x + empty_info.distance;
    Mat middle ;//检测区域图
//    middle.create(emp_img.size(),CV_8UC3);
//    copy(emp_img,middle,NULL);
    middle =emp_img.clone();
    for (int y = empty_info.start_p.y; y < empty_info.end_p.y; y++) {
        for (int x = empty_info.start_p.x; x < empty_info.end_p.x; x++) {

                if(x< circle_occupy_top_w && y < circle_occupy_top_w){
                    MpixelB(middle ,x,y)=0; MpixelG(middle ,x,y)=255; MpixelR(middle ,x,y)=0;//green
                } else if (x < circle_occupy_top_w  && y > circle_occupy_bottom_w  ){
//                cout << "左下圈圈: " << x << ","<<y<< endl;
                    MpixelB(middle ,x,y)=255; MpixelG(middle ,x,y)=0; MpixelR(middle ,x,y)=0;//blue

                }else if(x > circle_occupy_bottom_w  && y >circle_occupy_bottom_w ){
//                cout << "右下圈圈: " << x << ","<<y<< endl;
                    MpixelB(middle ,x,y)=0; MpixelG(middle ,x,y)=255; MpixelR(middle ,x,y)=255;//yellow
                }else{
                    MpixelB(middle ,x,y)=0; MpixelG(middle ,x,y)=0; MpixelR(middle ,x,y)=255;//red
                }
//            circle(src, Point(c[0], c[1]), 2, scalar);
        }
    }
    imshow("middle",middle);
    vector<PointInfo> empty_points;



    imwrite("./results/resized_img.jpg",dst_img);
//    vector<Point2f> input_resized_3_point = find_points(resized_img,Scalar(255,255,0));
//    cout <<"input_resized_3_point:" <<input_resized_3_point<<endl;



    waitKey(0);
    return 1;

}
#endif


