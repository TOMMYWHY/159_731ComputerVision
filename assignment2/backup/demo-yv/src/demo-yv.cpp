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
        Vec3i c = circles[i];
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


Mat threshold_2_8_color(Mat org_img){
    Mat threshold_img ;
    threshold_img.create(org_img.size(),CV_8UC3);
    Vec3b new_pixel_pre;
    for (int x=0;x<org_img.cols;x++){
        for ( int y=0;y<org_img.rows ; y++) {
            Vec3b pixel_image = org_img.at<Vec3b>(y, x);
            Vec3b pixel_image2 = org_img.at<Vec3b>(y, x);

            pixel_image2[0]=pixel_image[0] >128 ? 255:0;
            pixel_image2[1]=pixel_image[1] >128 ? 255:0;
            pixel_image2[2]=pixel_image[2] >128 ? 255:0;
            threshold_img.at<Vec3b>(y,x)=pixel_image2; // 颜色校正
        }
    }
    imshow("threshold_2_8_color",threshold_img);
    return threshold_img;
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

//void points_2_vector_for_dot(vector<Point2f> turn_up_Points,vector<Point2f> emp_circles){}
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

void vectors_dot(){}
#if 000001
int main (int argc, char* argv[]){
    Mat emp_img = imread("./images/2Dempty.jpg",1);

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
    Mat input_img = imread("./images/farfaraway_rotated.jpg",1);
//    Mat input_img = imread("./images/farfaraway_scaled.jpg",1); //18
//    Mat input_img = imread("./images/farfaraway_rotated_scaled.jpg",1); //18

//    imshow("emp_img",emp_img);
    imshow("input_img",input_img);


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

/*从新获取调整后图 3个点位置，并确定直角点位为 1号。*/

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

            if(x< nth_s && y < nth_s){
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
/*轮廓检测 03*/
#if 00000

int main (int argc, char* argv[]){
    Mat img = imread("./images/2Dempty.jpg");
//    Mat img = imread("./images/abcde.jpg ");
//    Mat img = imread("./images/abcde_rotated_scaled.jpg");

//=============颜色 阈值化==================
/*
    Mat input_threshold ;
    input_threshold.create(img.size(),CV_8UC3);
    Vec3b new_pixel_pre;
    for (int x=0;x<img.cols;x++){
        for ( int y=0;y<img.rows ; y++) {
            Vec3b pixel_image = img.at<Vec3b>(y, x);
            Vec3b pixel_image2 = img.at<Vec3b>(y, x);

            pixel_image2[0]=pixel_image[0] >128 ? 255:0;
            pixel_image2[1]=pixel_image[1] >128 ? 255:0;
            pixel_image2[2]=pixel_image[2] >128 ? 255:0;
            input_threshold.at<Vec3b>(y,x)=pixel_image2; // 颜色校正
        }
    }
*/

    Mat gray, mid, canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    GaussianBlur(img,mid,Size(3,3),0);

    Canny(mid, canny_output, 80, 255, 3);
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    Mat imageContours=Mat::zeros(img.size(),CV_8UC1);
    Mat Contours=Mat::zeros(img.size(),CV_8UC1);  //绘制

    vector<Point>  rect_center;
    double minarea = 0;
    double maxarea = 0;
    int maxAreaIdx = 0;
    double whRatio =0;

    for (int j = 0; j < hierarchy.size(); j++) {
//        cout <<"hierarchy:"<< hierarchy[j]<<endl;
    }

    for (int i = 0; i<contours.size(); i++)
    {
//        cout << contourArea(contours[i]) <<endl;
//        cout << fabs(contourArea(contours[i]))<<endl;




//        double tmparea = fabs(contourArea(contours[i]));
        double con_area = fabs(contourArea(contours[i]));
        double con_meter =arcLength(contours[i], true);
        cout <<"" fabs(contourArea(contours[i]))<<endl;

        if (con_area>maxarea)
        {
            maxarea = con_area;
            maxAreaIdx = i;
            continue;
        }
//        cout <<"maxarea:"<< maxarea<<endl;


        /*if (con_area < minarea)
        {
            //删除面积小于设定值的轮廓
            contours.erase(contours.begin() + i);
            std::wcout << "delete a small area" << std::endl;
            continue;
        }*/
        //计算轮廓的直径宽高
        Rect aRect =boundingRect(Mat(contours[i]));
//        cout <<"aRect:"<< aRect<<endl;
//        float avgX = (aRect.x + aRect.x + aRect.width)/2; //运动物体的矩形的中点X位置
//        float avgY = (aRect.y + aRect.y + aRect.height)/2;//运动物体的矩形的中点Y位置

        float avgX = (aRect.x + aRect.x + aRect.width)/2; //运动物体的矩形的中点X位置
        float avgY = (aRect.y + aRect.y + aRect.height)/2;//运动物体的矩形的中点Y位置

//        cout<<"x:"<<avgX<<"y:"<<avgY<<endl;
        if(aRect.width   ){
            contours.erase(contours.begin() + i);
//            std::wcout << "aRect.width a small area" << std::endl;
            rect_center.push_back(Point(avgX,avgY));

            continue;
        }
//         if ((aRect.width / aRect.height)<whRatio)
         /*if (abs(aRect.width - aRect.height)  )
         {
             //删除宽高比例小于设定值的轮廓
             contours.erase(contours.begin() + i);
             std::wcout << "delete a unnomalRatio area" << std::endl;
             continue;
         }*/
    }

    for( size_t i = 0; i < rect_center.size(); i++ ) {
        Vec2i c = rect_center[i];
        circle( img, Point(c[0], c[1]), 8, Scalar(0,0,255) , 1, 1);
//        circle(src, Point(c[0], c[1]), 2, Scalar(0,0,255));
//        center_point.push_back(Point(c[0], c[1]));
    }
    imshow("img",img);

    /// Draw contours,彩色轮廓
    Mat dst= Mat::zeros(canny_output.size(), CV_8UC3);
    for (int i = 0; i< contours.size(); i++)
    {
        //随机颜色
        Scalar color = Scalar(0,0,255);
        drawContours(dst, contours, i, color, 2, 8, hierarchy, 0, Point());
    }


    imshow("dst",dst);



    /*  for(int i=0;i<contours.size();i++)
      {
          //contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
  //        cout <<"contours[i].size() :"<< contours[i].size() <<endl;
          for(int j=0;j<contours[i].size();j++)
          {
              //绘制出contours向量内所有的像素点
              Point P=Point(contours[i][j].x,contours[i][j].y);
              Contours.at<uchar>(P)=255;
          }

          //输出hierarchy向量内容
          char ch[256];
          sprintf(ch,"%d",i);
          string str=ch;
          cout<<"向量hierarchy的第" <<str<<" 个元素内容为："<<endl<<hierarchy[i]<<endl<<endl;

          //绘制轮廓
          drawContours(imageContours,contours,i,Scalar(255),1,8,hierarchy);
      }
      imshow("Contours Image",imageContours); //轮廓
      imshow("Point of Contour",Contours);   //向量contours内保存的所有轮廓点集
  */


    /*Mat hsv, mask;
    cvtColor(input_threshold, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(40,43, 46), Scalar(50, 255, 255), mask);
    imshow("mask", mask);

    Mat se = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
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
        circle(input_threshold, ct, 2, Scalar(0, 0, 255), 2, 8);
        circle(input_threshold, ct, (h + w) / 4, Scalar(255, 0, 0), 2, 8, 0);
    }


    imshow("input_threshold",input_threshold);
    imshow("img",img);*/


//    Scalar red = Scalar(255,0,0);
//    cvtColor()
    waitKey(0);
    return 1;

}
#endif
/*轮廓检测 03*/
#if 000000

int main (int argc, char* argv[]){
    Mat emp_img = imread("./images/2Dempty.jpg");
    Mat test;
//    test=emp_img.clone();
    test.create( emp_img.size(), CV_8UC3);

    int each_w =20;
    int padding =  each_w * 6 - 90; // 白边
    int nth_s = each_w * 6 +30; //nothing there 近距离
    int nth_l =820 +30; //nothing there 远距离
    cout <<"nth_s:"<<nth_s<< endl;
    for (int x = 30; x < emp_img.cols-30; x++) {
        for (int y = 30; y <emp_img.rows-30; y++) {
            if(x< nth_s && y < nth_s){
//                cout << "左上圈圈: " << x << ","<<y<< endl;
                MpixelB(test ,x,y)=0; MpixelG(test ,x,y)=0; MpixelR(test ,x,y)=255;
            } else if (x < nth_s  && y > nth_l  ){
                MpixelB(test ,x,y)=0; MpixelG(test ,x,y)=0; MpixelR(test ,x,y)=255;
            }else if(x > nth_l  && y >nth_l ){
//                cout << "右下圈圈: " << x << ","<<y<< endl;
                MpixelB(test ,x,y)=0; MpixelG(test ,x,y)=0; MpixelR(test ,x,y)=255;
            }else{
                /*if(x%20==3 && y%20==3 || x%20==4 && y%20==4 || x%20==5 && y%20==5 || x%20==6 && y%20==6 || x%20==7 && y%20==7){
                    cout << "采样xy: " << x << ","<<y<< endl;
                    cout << "bgr: " << int(MpixelB(emp_img ,x,y)) << ","<< int(MpixelG (emp_img,x,y))<< ","<<int(MpixelR ( emp_img , x , y ))<< endl;
                    MpixelB(test ,x,y)      =MpixelB(emp_img ,x,y) ;
                    MpixelG ( test , x , y )=MpixelG ( emp_img , x , y ) ;
                    MpixelR ( test , x , y )=  MpixelR ( emp_img , x , y ) ;
                }else{
                    MpixelB(test ,x,y)      =  255;//MpixelB(emp_img ,x,y) ;
                    MpixelG ( test , x , y )=  0;//MpixelG ( emp_img , x , y ) ;
                    MpixelR ( test , x , y )=  255;//MpixelR ( emp_img , x , y ) ;
                }*/
                MpixelB(test ,x,y)      =  0;//MpixelB(emp_img ,x,y) ;
                MpixelG ( test , x , y )=  0;//MpixelG ( emp_img , x , y ) ;
                MpixelR ( test , x , y )=  255;//MpixelR ( emp_img , x , y ) ;

            }

            /*if(x == y){
                MpixelB(test ,x,y)=0; MpixelG(test ,x,y)=0; MpixelR(test ,x,y)=255;
            } else{
                MpixelB(test ,x,y)=MpixelB(emp_img ,x,y) ;
                MpixelG ( test , x , y )=MpixelG ( emp_img , x , y ) ;
                MpixelR ( test , x , y )=MpixelR ( emp_img , x , y ) ;
            }*/
        }
    }


 /*   for (int x=30;x<emp_img.cols-30;x++){
        for ( int y=30;y<emp_img.rows-30 ; y++) {
            Vec3b pixel_image = emp_img.at<Vec3b>(y, x);
//            Vec3b pixel_image2 = input_threshold.at<Vec3b>(y, x);

            if(x< nth_s && y < nth_s){
//                cout << "左上圈圈: " << x << ","<<y<< endl;
                pixel_image[0] = 255;pixel_image[1] = 255;pixel_image[2] = 255;
            } else if (x < nth_s  && y > nth_l  ){
//                cout << "左下圈圈: " << x << ","<<y<< endl;
                pixel_image[0] = 255;pixel_image[1] = 255;pixel_image[2] = 255;

            }else if(x > nth_l  && y >nth_l ){
//                cout << "右下圈圈: " << x << ","<<y<< endl;
                pixel_image[0] = 255;pixel_image[1] = 255;pixel_image[2] = 255;

            } else{
                if(x%20==3 && y%20==3 || x%20==4 && y%20==4 || x%20==5 && y%20==5 || x%20==6 && y%20==6 || x%20==7 && y%20==7)
//                if( x%20==5 && y%20==5 )
                {
                    cout << "采样xy: " << x << ","<<y<< endl;
                    cout << "bgr: " << pixel_image[0] << ","<<pixel_image[1] << ","<<pixel_image[2] << endl;

                    //TODO

//                    pixel_image[0] = 255;pixel_image[1] = 0;pixel_image[2] = 255;
                    test.at<Vec3b>(y,x)=pixel_image;
                    int b = pixel_image[0];
                    int g = pixel_image[1];
                    int r = pixel_image[2];
//                    cout << "bgr: " << b << ","<<g << ","<<r << endl;

                    b=b>128 ? 1:0;
                    g=g>128 ? 1:0;
                    r=r>128 ? 1:0;
                    string s_b=to_string(b);
                    string s_g=to_string(g);
                    string s_r=to_string(r);
                    string res = s_b+s_g+s_r;
            cout << "res:"<< res <<endl;
                    decode_img += res;
                }else{
                    pixel_image[0] = 0;pixel_image[1] = 0;pixel_image[2] = 255;
                }
            }
            test.at<Vec3b>(y,x)=pixel_image; // 颜色校正
        }
    }*/

    imshow("test",test);

    waitKey(0);
    return 1;

}
#endif