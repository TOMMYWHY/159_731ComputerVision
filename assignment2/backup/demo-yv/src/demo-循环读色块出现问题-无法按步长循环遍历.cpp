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
    cout <<degree_alignment <<endl;
    cv::Point2f input_center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_alignment = cv::getRotationMatrix2D(input_center, degree_alignment, 1.0);
    cv::warpAffine(input_img, input_alignment, rot_alignment, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    imshow("input_alignment",input_alignment);
    return input_alignment;
}

#if 0001
int main (int argc, char* argv[]){
    Mat emp_img = imread("./images/2Dempty.jpg",1);
    Mat input_img = imread("./images/abcde_rotated.jpg",1);
//    Mat input_img = imread("./images/congratulations_rotated_scaled.jpg",1);
    imshow("input_img",input_img);

//    调正input
    Mat input_alignment = fine_tuning(input_img);


    vector<Point2f> emp_circles = find_points(emp_img);
/*找到三个点位置*/
    Point2f dstTri[3] = {emp_circles[0],emp_circles[1],emp_circles[2]}; //empty
    /*line(emp_img, dstTri[0],dstTri[1], Scalar(0,0,255), 3);
    line(emp_img, dstTri[1],dstTri[2], Scalar(0,0,255), 3);
    line(emp_img, dstTri[2],dstTri[0], Scalar(0,0,255), 3);*/

/*找直角索引*/
    //  已摆正
    int empty_right_angle=  right_angle_points(emp_circles);
    cout << "wocao, empty right-angle point is : "<<empty_right_angle  << endl;

/*两张图片的中心点*/
    int img_height = input_img.rows;
    int img_width = input_img.cols;
    int empty_img_height = emp_img.rows;
    int empty_img_width = emp_img.cols;

/*调正后图片直角点坐标*/
    vector<Point2f> turn_up_circles = find_points(input_alignment);
    int turn_up_right_angle=  right_angle_points(turn_up_circles);
    Point2f turn_up_Tri[3] = {turn_up_circles[0],turn_up_circles[1],turn_up_circles[2]}; //empty
    cout << "turn_up_right_angle:"<<turn_up_Tri[turn_up_right_angle]<<endl;

    /*计算直角边距离*/
    Point2f srcTri[3] ={turn_up_circles[0],turn_up_circles[1],turn_up_circles[2]};; //input

    float dis;
    if(turn_up_right_angle!=0){
        dis = powf((srcTri[turn_up_right_angle].x - srcTri[0].x),2) + powf((srcTri[turn_up_right_angle].y - srcTri[0].y),2);
    }else{
        dis = powf((srcTri[turn_up_right_angle].x - srcTri[1].x),2) + powf((srcTri[turn_up_right_angle].y - srcTri[1].y),2);
    }
    dis = sqrtf(dis);
    cout << "dis:"<<dis<<endl;

/*旋转90度倍数*/
    int angle=0; //滚动角度
    if(turn_up_Tri[turn_up_right_angle].x < dis && turn_up_Tri[turn_up_right_angle].y> dis){
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
    cv::Point2f center(static_cast<float>(input_alignment.cols / 2.), static_cast<float>(input_alignment.rows / 2.));
    Mat M = getRotationMatrix2D(center,angle,1);//计算旋转的仿射变换矩阵
//    warpAffine(turn_up_img,dst_img,M,Size(turn_up_img.cols,turn_up_img.rows));//仿射变换
    cv::warpAffine(input_alignment, dst_img, M, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    imshow("dst_img",dst_img);



//=============颜色 阈值化==================
    Mat input_threshold ;
    input_threshold.create(dst_img.size(),CV_8UC3);
    Vec3b new_pixel_pre;
    for (int x=0;x<dst_img.cols;x++){
        for ( int y=0;y<dst_img.rows ; y++) {
            Vec3b pixel_image = dst_img.at<Vec3b>(y, x);
            Vec3b pixel_image2 = dst_img.at<Vec3b>(y, x);

            pixel_image2[0]=pixel_image[0] >128 ? 255:0;
            pixel_image2[1]=pixel_image[1] >128 ? 255:0;
            pixel_image2[2]=pixel_image[2] >128 ? 255:0;
            input_threshold.at<Vec3b>(y,x)=pixel_image2; // 颜色校正
        }
    }
    imshow("input_threshold",input_threshold);






//    imshow("emp_img",emp_img);
//    imshow("input_img",input_img);
//    imshow("warp_dst",warp_dst);
//    imshow("input_adjust",input_adjust);
//    imshow("img_trans",img_trans);

//    imshow("Circles detected", gray_img);
    waitKey(0);
    return 1;

}
#endif


/*轮廓检测*/
/*使用findcountours 必须使用empty图， 因为彩色图会出现黑色连接问题。
 * 所以需要 用仿射 将图片旋转至emtpy同一位置，才能使用findcountours。。。。
 * */
#if 000

RNG rng(12345);
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


int main (int argc, char* argv[]){
//    Mat img = imread("./images/lena.jpg");
    Mat img = imread("./images/2Dempty.jpg",1);
//    Mat img = imread("./images/abcde.jpg",1);
//    Mat img = imread("./images/Darwin_rota ted.jpg",1);



    Mat greyimage, binary_img,img_temp;
  // todo 加一层高斯 或者 中值滤波


//    cvtColor(img, greyimage, COLOR_BGR2GRAY);

//     greyimage.copyTo(img_temp);
//    Canny(img, binary_img, 30,100,3);
    blur(img, img, Size(3, 3));
    Canny(img, binary_img, 80, 255, 3);
    imshow("binary_img", binary_img);
//    threshold(greyimage, binary_img, 0, 255, THRESH_BINARY);
    vector<vector<Point> > contours;
    vector<Point>  rect_center;
    vector<Vec4i> hierarchy;

    findContours(binary_img,contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    Mat drawing = Mat::zeros( binary_img.size(), CV_8UC3 );

    cout <<contours.size() <<endl;
    double maxarea = 0;
    int maxAreaIdx = 0;
    double minarea = 30;
    double whRatio = 1;
 /*   for (int i = 0; i<contours.size(); i++)
    {
                    std::wcout << "fabs contourArea(contours[i]):" <<  fabs(contourArea(contours[i]))<<std::endl;


//        double tmparea = fabs(contourArea(contours[i]));
        double tmparea = contourArea(contours[i]);

        if(tmparea > 100 ){
            contours.erase(contours.begin() + i);
            continue;
        }

        //计算轮廓的直径宽高
        Rect aRect =boundingRect(contours[i]);

        cout <<"aRect:"<< aRect<<endl;
        if ((aRect.width / aRect.height)<whRatio)
        {
            //删除宽高比例小于设定值的轮廓
            contours.erase(contours.begin() + i);
            std::wcout << "delete a unnomalRatio area" << std::endl;
            continue;
        }
        RotatedRect box =minAreaRect(contours[i]);
        Point2f vertices[4];
        box.points(vertices);
        Point2f center_box;
        center_box.x = (vertices[0].x+vertices[2].x)/2.0;
        center_box.y = (vertices[0].y+vertices[2].y)/2.0;
        rect_center.push_back(center_box);
    }*/

    for (int i = 0; i<contours.size(); i++)
    {
//        contourArea(contours[i]);
//        std::wcout << "contourArea(contours[i]);" <<contourArea(contours[i])<< std::endl;

        double tmparea = fabs(contourArea(contours[i]));
        if (tmparea>maxarea)
        {
            maxarea = tmparea;
            maxAreaIdx = i;
            continue;
        }

        if (tmparea < minarea || tmparea > 500 )
        {
            //删除面积小于设定值的轮廓
            contours.erase(contours.begin() + i);
            std::wcout << "delete a small area" << std::endl;
            continue;
        }
        //计算轮廓的直径宽高
        Rect aRect =boundingRect(contours[i]);
        Point2f center_box= Point2f(aRect.x+int(aRect.width/2),aRect.y+int(aRect.height/2));
//        Point2f center_box= Point2f(aRect.x+5,aRect.y+5);
        rect_center.push_back(center_box);
        cout <<"center_box:"<<center_box<<endl;
        if ((aRect.width / aRect.height)<whRatio)
        {
            //删除宽高比例小于设定值的轮廓
            contours.erase(contours.begin() + i);
            std::wcout << "delete a unnomalRatio area---------------" << std::endl;
            continue;
        }else{
            Point2f center_box= Point2f(aRect.x+aRect.width/2.0,aRect.y+aRect.height/2.0);
            rect_center.push_back(center_box);
            cout <<"center_box:"<<center_box<<endl;
        }


    }

//    cout <<rect_center.size()<<endl;
//    cout <<maxAreaIdx<<endl;
    cout <<contours.size() <<endl;


    for( size_t i = 0; i < rect_center.size(); i++ ) {
        Vec2i c = rect_center[i];
        circle( img, Point(c[0], c[1]), 2, Scalar(255,0,0) , 1, 1);
//        circle(src, Point(c[0], c[1]), 2, Scalar(0,0,255));
//        center_point.push_back(Point(c[0], c[1]));
    }

    Mat dst= Mat::zeros(binary_img.size(), CV_8UC3);
    for (int i = 0; i< contours.size(); i++)
    {
        //随机颜色
//        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        Scalar color = Scalar(200+i/255,200+i/255,200+i/255);
        drawContours(dst, contours, i, color, 2, 2, hierarchy, 0, Point());
    }
    imshow("dst",dst);





/*    for( size_t i = 0; i< contours.size(); i++ )
    {
        cout << contours[i] <<endl;
//        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        drawContours( drawing, contours, (int)i, Scalar(0,0,255), 1, LINE_8, hierarchy, 0 );
    }*/
//    imshow( "Contours", drawing );



    imshow("img",img);
//    imshow("greyimage",greyimage);
//    imshow("binary_img",binary_img);

    waitKey(0);
    return 1;

}
#endif

/*轮廓检测 02*/
#if 000

int iLowH = 50,iHighH=150,iLowS = 50,iHighS=150,iLowV = 50,iHighV=150,g_cannyLowThreshold=50;
int main (int argc, char* argv[]){
    Mat img = imread("./images/test.jpg");
//    Mat img = imread("./images/2Dempty.jpg",1);
//    Mat img = imread("./images/abcde.jpg",1);
//    Mat img = imread("./images/Darwin_rotated.jpg",1);

    namedWindow("color");
    createTrackbar("iLowH","color",&iLowH,179);
    createTrackbar("iHighH","color",&iHighH,179);
    createTrackbar("iLowS","color",&iLowS,255);
    createTrackbar("iHighS","color",&iHighS,255);
    createTrackbar("iLowV","color",&iLowV,255);
    createTrackbar("iHighV","color",&iHighV,255);

    Mat imgHSV, imgThresholded,imgCanny,element;
    double length_max=0;
    vector<Mat> hsvSplit;
    vector<vector<Point>> contours;
    cvtColor(img,imgHSV,COLOR_BGR2HSV);
    inRange(imgHSV, Scalar(iLowH,iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
    element = getStructuringElement(MORPH_RECT,Size(3,3));
    morphologyEx(imgThresholded,imgThresholded,MORPH_OPEN,element);
    morphologyEx(imgThresholded,imgThresholded,MORPH_CLOSE,element);
//    Canny(imgThresholded,imgCanny,g_cannyLowThreshold,g_cannyLowThreshold*3);
//    Canny(imgThresholded, imgCanny, 50,100,3);
    findContours(imgThresholded, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int i,k;
    if(contours.size()>0){
        k=0;
        for (int i = 0; i < contours.size(); i++) {
            if(arcLength(contours[i],0)>length_max){
                length_max = arcLength(contours[i],0);
                k=i;
            }
        }

        drawContours(img,contours,k,Scalar(0,0,255),4);
        Point2f vertices[4];
        Point2f center_box;
        RotatedRect box = minAreaRect(contours[k]);
        box.points(vertices);
        center_box.x = (vertices[0].x+vertices[2].x)/2.0;
        center_box.y = (vertices[0].y+vertices[2].y)/2.0;
//        rect_center.push_back(center_box);
        circle(img,center_box,3,Scalar(0,255,0),-1,8,0);
        cout <<160-center_box.x<<","<< 120-center_box.y<<endl;
        cout <<"center_box:"<<center_box<<endl;
    }

    imshow("color",imgThresholded);
    imshow("img",img);


  /*  Mat greyimage, binary_img,img_temp;
    Mat hsv, mask;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(40,43, 46), Scalar(50, 255, 255), mask);
    imshow("mask", mask);*/
   /* Mat se = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
    morphologyEx(mask, mask, MORPH_OPEN, se);
     imshow("binary", mask);

    vector<vector<Point>> contours;
    vector<Vec4i> hiearchy;
    findContours(mask, contours, hiearchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); i++) {
             // 圆拟合
             RotatedRect rrt = fitEllipse(contours[i]);
             Point ct = rrt.center;
             int h = rrt.size.height;
             int w = rrt.size.width;
             printf("height : %d, width : %d \n", h, w);
              circle(img, ct, 2, Scalar(0, 0, 255), 2, 8);
        circle(img, ct, (h + w) / 4, Scalar(255, 0, 0), 2, 8, 0);
    }
*/
//    imshow("img",img);
//    imshow("hsv",hsv);
//    imshow("greyimage",greyimage);
//    imshow("binary_img",binary_img);

    waitKey(0);
    return 1;

}
#endif
