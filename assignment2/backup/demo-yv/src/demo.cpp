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


vector<Point2f>  find_points(Mat src , int thred_1,int thred_2, Scalar scalar){
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
    cout <<"fun:right_angle_points ->AA.dot(BB)" <<abs(AA.dot(BB))<<endl;
    cout <<"fun:right_angle_points ->BB.dot(CC)" <<abs(BB.dot(CC))<<endl;
    cout <<"fun:right_angle_points ->CC.dot(AA)" <<abs( CC.dot(AA))<<endl;

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
//    cout <<"fun:right_angle_points ->min" <<min<<endl;
//    cout <<"fun:right_angle_points ->" << right_angle_flag<<endl;
//    return right_angle;
    return right_angle_index;
}
/*将图片调正*/
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
    cout <<"fine tuning angle degree (first rotate) : " <<degree_alignment <<endl;
    cv::Point2f input_center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_alignment = cv::getRotationMatrix2D(input_center, degree_alignment, 1.0);
    cv::warpAffine(input_img, input_alignment, rot_alignment, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
//    imshow("input_alignment",input_alignment);
    return input_alignment;
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

vector<Point2f> reposition_right_angle(vector<Point2f> turn_up_circles ,int right_angle_index){
    Point2f turn_up_Tri[3] = {turn_up_circles[0],turn_up_circles[1],turn_up_circles[2]};
    cout << "fun : reposition_right_angle -> turn_up_right_angle:"<<turn_up_Tri[right_angle_index]<<endl;
    vector<Point2f> turn_up_Points_vct = {turn_up_circles[0],turn_up_circles[1],turn_up_circles[2]}; // temp 无序点
//    cout << "-------inputPoints ------ : "<<turn_up_Points_vct << endl;
//    cout << "-------turn_up_right_angle ------ : "<<right_angle_index << endl;
    turn_up_Points_vct.erase(turn_up_Points_vct.begin()+right_angle_index); // 先删除直角点
//    cout << "-------after erase  inputPoints 2 point------ : "<<turn_up_Points_vct << endl;
    turn_up_Points_vct.insert(turn_up_Points_vct.begin()+1, turn_up_Tri[right_angle_index]);//在1号位加入直角点
//    cout << "-------after insert  inputPoints ------ : "<<turn_up_Points_vct << endl; // 此时 中心为直角点
//    cout << "===========: "<< endl;
    return turn_up_Points_vct;
}

vector<double> vectors_dot(vector<Point2f> turn_up_Points,vector<Point2f> emp_circles, double input_dis, double emp_dis){
    Vec2f p_i_0(turn_up_Points[0].x, -turn_up_Points[0].y), p_i_1(turn_up_Points[1].x, -turn_up_Points[1].y),p_i_2(turn_up_Points[2].x,-turn_up_Points[2].y);
    Vec2f p_e_0(emp_circles[0].x, -emp_circles[0].y), p_e_1(emp_circles[1].x, -emp_circles[1].y),p_e_2(emp_circles[2].x,-emp_circles[2].y);
    Vec2f i_p1_p0 = p_i_0 - p_i_1;
    Vec2f e_p1_p0 = p_e_0 - p_e_1;

    Vec2f i_p1_p2 = p_i_2 - p_i_1;
    Vec2f e_p1_p2 = p_e_2 - p_e_1;

    double cos_theta_A =( i_p1_p0.dot(e_p1_p0)/ (emp_dis * input_dis) );
    double cos_theta_B =( i_p1_p2.dot(e_p1_p2)/ (emp_dis * input_dis) );
    vector<double> thetas = {cos_theta_A,cos_theta_B};
    return thetas;
}

#if 000001
int main (int argc, char* argv[]){

    /*if (argc != 2) {printf("needs an input image\n");exit(0);}
    Mat input_img = imread ( argv [ 1 ] , 1) ;*/

    Mat emp_img = imread("./images/2Dempty.jpg",1);

//    Mat input_img = imread("./images/abcde.jpg",1);
    Mat input_img = imread("./images/abcde_rotated.jpg",1);
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
//    Mat input_img = imread("./images/farfaraway_rotated.jpg",1); // todo 19
//    Mat input_img = imread("./images/farfaraway_scaled.jpg",1); //18
//    Mat input_img = imread("./images/farfaraway_rotated_scaled.jpg",1); //18

//    imshow("emp_img",emp_img);
    imshow("input_img",input_img);

//    调正input todo planB 可以不调正 直接计算正图与input向量夹角
    vector<Point2f> temp_input_circles = find_points(input_img, 100,30 ,Scalar(255,255,0));
    imshow("input_img",input_img);

    Mat input_alignment = fine_tuning(input_img);
    imshow("input_alignment",input_alignment);

    // 找到空图点位置
    vector<Point2f> emp_circles = find_points(emp_img, 100,30 ,Scalar(0,0,0));
/*空图三个点位置*/
    Point2f dstTri[3] = {emp_circles[0],emp_circles[1],emp_circles[2]}; //empty
    imshow("emp_img",emp_img);
    //[92, 90; 90, 910; 910, 910]
/*找直角索引*/
    //  已摆正
    int empty_right_angle=  right_angle_points(emp_circles);
    cout << "empty right-angle point index is : "<<empty_right_angle  << endl;

/*调正后图片直角点坐标 在1号位*/
    vector<Point2f> turn_up_circles = find_points(input_alignment,100,30,Scalar(0,0,255));
    cout<<"???"<<turn_up_circles << endl;
    if(turn_up_circles[0].x==0 && turn_up_circles[1].x==0 && turn_up_circles[2].x==0){
        return -1;
    }
    int turn_up_right_angle =  right_angle_points(turn_up_circles);
    cout <<"turn_up_right_angle:"<< turn_up_right_angle<<endl;
    /*if(turn_up_right_angle == empty_right_angle){
        cout << " first rotate correct " << endl;
    }else{
        cout << " need one more rotate~! " << endl;
    }*/
    cout << "------------------------------ ------ : " << endl;
    vector<Point2f>  turn_up_Points_vct = reposition_right_angle(turn_up_circles,turn_up_right_angle);
    cout << " input_alignment right-angle point index is : "<<turn_up_right_angle  << endl;

    cout << "turn_up_Points_vct =>"<< turn_up_Points_vct<<endl; // 确保 直角点在 1号位

/*计算直角点距离*/
    double emp_dis = sqrt(powf((emp_circles[1].x - emp_circles[0].x),2) + powf((emp_circles[1].y - emp_circles[0].y),2));
    double input_dis = sqrt(powf((turn_up_Points_vct[1].x - turn_up_Points_vct[0].x),2) + powf((turn_up_Points_vct[1].y - turn_up_Points_vct[0].y),2));
    cout << "emp_dis: "<<emp_dis << endl;
    cout << "input_dis: "<<input_dis << endl;
//    cout << "turn_up_Points_vct =>"<< turn_up_Points_vct<<endl;
    int angle=0;
    if(turn_up_Points_vct[1].x<input_dis && turn_up_Points_vct[1].y > input_dis){
        cout << "正图"<<endl;// 一号位在左下 x 小，y 大
    }else if(turn_up_Points_vct[1].x<input_dis && turn_up_Points_vct[1].y < input_dis){
        cout << "转90"<<endl;
        angle=90;
    }else if(turn_up_Points_vct[1].x>input_dis && turn_up_Points_vct[1].y<input_dis){
        cout << "转180"<<endl;
        angle=180;
    }else if(turn_up_Points_vct[1].x>input_dis && turn_up_Points_vct[1].y>input_dis){
        cout << "转270"<<endl;
        angle=270;
    }

    cv::Point2f center(static_cast<float>(input_alignment.cols / 2.), static_cast<float>(input_alignment.rows / 2.));
    Mat dst_img;
    Mat M = getRotationMatrix2D(center,angle,1);//计算旋转的仿射变换矩阵
    warpAffine(input_alignment ,dst_img,M,Size(input_alignment.cols,input_alignment.rows));//仿射变换
    imshow("dst_img1",dst_img); //图已经调正
    cout << "------------------------------ ------ : " << endl;
    cout << "------------------------------ ------ : " << endl;
    cout << "------------------------------ ------ : " << endl;
    cout << "------------------------------ ------ : " << endl;

    /* 此时 图片不是 100% 正图 需要在调整一次*/
//    dst_img = fine_tuning(dst_img);

/*从新获取调整后图 3个点位置，并确定直角点位为 1号。*/ //todo 根据旋转角度 计算出新3点位置

    vector<Point2f> dst_circles = find_points(dst_img ,100,50, Scalar(255,255,255));
    cout << "dst_img  dst_circles: " << dst_circles<< endl;

    int dis_right_angle_index=  right_angle_points(dst_circles);
    cout << "dis_right_angle_index: " << dis_right_angle_index<< endl;

    vector<Point2f>  dis_Points_vct = reposition_right_angle(dst_circles,dis_right_angle_index);
    cout << "dis_Points_vct: " << dis_Points_vct<< endl;


//    cout << "dst_circles: " << dst_circles<< endl;
    cout << "emp_circles: " << emp_circles<< endl;
    double dst_dis = sqrt(powf((dis_Points_vct[1].x - dis_Points_vct[0].x),2) + powf((dis_Points_vct[1].y - dis_Points_vct[0].y),2));

    cout << "dst_dis: " << dst_dis<< endl;
    cout << "emp_dis: " << emp_dis<< endl;
    cout << "input_dis: " << input_dis<< endl;

//    Mat input_threshold = threshold_2_8_color(dst_img);//将色块归一化为8个值
    Mat input_threshold = dst_img;//
//===============================
    int circle_occupy_sqt = 6; // 圆所占格子数

    /* 47*47 */
    int each_w =int( dst_dis /(47-circle_occupy_sqt));//dis = l - 2*r;  2*r = circle_occupy_sqt
    Point scan_begin,scan_end;

    scan_begin.x =dis_Points_vct[1].x-3*each_w; // r = 3*each_w //通过直角点计算 起始点坐标
    scan_begin.y =dis_Points_vct[1].y - dst_dis-3*each_w;
    scan_end.x=scan_begin.x+dst_dis+6*each_w;
    scan_end.y=scan_begin.y+dst_dis+6*each_w;
    cout << "!!!!scan_begin"<<scan_begin<<endl;
    cout << "!!!!scan_end"<<scan_end<<endl;

    cout << "each_w: " << each_w<< endl;
//    cout << "padding: " << padding<< endl;
    int nth_s = scan_begin.x + each_w*circle_occupy_sqt; //nothing there short近距离 // 圆所占位置
    int nth_l =scan_begin.x + int(dst_dis) ; //nothing there long远距离
//    cout << "emp_dis:"<< emp_dis<<endl;
    Mat test;//检测区域图
    vector<Point> std_points ;//
//    test.create(emp_img.size(),CV_8UC3);
    test.create(input_threshold.size(),CV_8UC3);
//    test=emp_img.clone();/
//    imshow("test0",test);

    string decode_img ;
    //TODO planB图片可以裁剪
//    imshow("emp_img",emp_img);
//    imshow("input_img",input_img);


    int max_width = dst_dis+ (each_w * 6);
    int step,shift;
    int get_point_count = 0;
    if(each_w > 20){
        step = int(each_w/8);
        shift = 1;
    }else if(each_w < 19 ){
//        step = int( sqrt(each_w))+1;
//        shift = -1;
        step = 2;
        shift = 10;
    }
    else{
        step =8;
        shift = 1;
    }
    cout <<"step : "<< step<<endl;
    cout <<"shift : "<< shift<<endl;
    for (int y=scan_begin.y;y<scan_end.y;y++){
        for ( int x=scan_begin.x;x<scan_end.x ; x++) {
            Vec3b pixel_image = input_threshold.at<Vec3b>(y, x);

            if(x< nth_s && y < nth_s - each_w){
//                cout << "左上圈圈: " << x << ","<<y<< endl;
                MpixelB(test ,x,y)=255; MpixelG(test ,x,y)=255; MpixelR(test ,x,y)=255;

            } else if (x < nth_s  && y > nth_l  ){
//                cout << "左下圈圈: " << x << ","<<y<< endl;
//                pixel_image[0] = 255;pixel_image[1] = 255;pixel_image[2] = 255;
                MpixelB(test ,x,y)=255; MpixelG(test ,x,y)=255; MpixelR(test ,x,y)=255;

            }else if(x > nth_l  && y >nth_l ){
//                cout << "右下圈圈: " << x << ","<<y<< endl;
//                pixel_image[0] = 255;pixel_image[1] = 255;pixel_image[2] = 255;
                MpixelB(test ,x,y)=255; MpixelG(test ,x,y)=255; MpixelR(test ,x,y)=255;

            } else{

                if( x%each_w==int(each_w/step) && y%each_w==each_w/step)
                    {
                    pixel_image[0] = 255;pixel_image[1] = 255;pixel_image[2] = 255;
                    get_point_count ++;
                    test.at<Vec3b>(y,x)=pixel_image;
                    int b = int(MpixelB(input_threshold ,x+shift,y+shift));
                    int g = int(MpixelG(input_threshold ,x+shift,y+shift));
                    int r = int(MpixelR(input_threshold ,x+shift,y+shift));

                    b=b>128 ? 1:0;
                    g=g>128 ? 1:0;
                    r=r>128 ? 1:0;
                    string s_b=to_string(b);
                    string s_g=to_string(g);
                    string s_r=to_string(r);
//                    string res = s_b+s_g+s_r;
                    string res = s_r+s_g+s_b;
//            cout << "res:"<< res <<endl;
                    decode_img += res;
                }else{
                pixel_image[0] = 0;pixel_image[1] = 0;pixel_image[2] = 255; //红区
                }
            }
            test.at<Vec3b>(y,x)=pixel_image; // 颜色校正
        }
    }
    cout <<"get_point_count:" <<get_point_count<<endl;

        imshow("test",test);

    cout <<"decode_img:" <<decode_img<<endl;


    string decode_to_char;
    for (int i = 0; i < decode_img.size(); i=i+6) {
        if (i%6==0){
            string sigle = decode_img.substr(i, 6);
            reverse(sigle.begin(),sigle.end()); // 翻转字符串
            const char* arr = sigle.data();
            int ret = convertBinaryToDecimal(arr); // 二进制从后往前读
            cout << sigle<<"=>"<< ret <<"  ;  "; // decode 码已被翻转
            decode_to_char +=encodingarray[ret];
        }
    }
    cout << "====="<< endl;
    cout << "====="<< endl;

    cout << decode_to_char<< endl;

    waitKey(0);
    return 1;

}
#endif


