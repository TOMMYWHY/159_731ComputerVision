//
// Created by Tommy on 2020-03-07.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include<algorithm>
#include "decoding.h"
#include <math.h>
using namespace std;
using namespace cv;

vector<Vec3f> point_locations;
/*vector<Vec3f>  get_point_locations(const char * url){
    Mat img_org = imread(url,1);
    Mat greyimage ;
    medianBlur(img_org, img_org, 5);
    cvtColor(img_org, greyimage, COLOR_BGR2GRAY);
    vector<Vec3f> circles ; // 定位点
    HoughCircles(greyimage , circles , HOUGH_GRADIENT, 1, 30, 100,30, 1, 30);
    cout<< circles[0] <<";" <<  circles[1] <<";" << circles[2] <<";" <<endl;
    return circles;
}*/
vector<Vec3f>  get_point_locations(Mat img){
//    Mat img_org = imread(url,1);
    Mat greyimage ;
    medianBlur(img, img, 5);
    cvtColor(img, greyimage, COLOR_BGR2GRAY);
    vector<Vec3f> circles ; // 定位点
    HoughCircles(greyimage , circles , HOUGH_GRADIENT, 1, 30, 100,30, 1, 30);
    cout<< circles[0] <<";" <<  circles[1] <<";" << circles[2] <<";" <<endl;
    return circles;
}

//==================================

/*

 */
#if 0
int main (int argc, char* argv[]){
    Mat srcImage = imread("./images/2Dempty.jpg",1);
    Mat midImage,dstImage;//临时变量和目标图的定义

    Canny(srcImage, midImage, 50, 200, 3);//进行一此canny边缘检测
//    cvtColor(midImage,dstImage, COLOR_BGR2GRAY);//转化边缘检测后的图为灰度图
    vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
    HoughLinesP(midImage, lines, 1, CV_PI/180, 80, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( srcImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, 1);

    }
    imshow("srcImage", srcImage);

    Mat test_img =imread( "./images/Darwin_rotated.jpg",0);
    imshow("test_img",test_img);

    IplImage *pSrcImage = cvLoadImage(image_name, CV_LOAD_IMAGE_UNCHANGED);


//
//    cv::Point2f center(static_cast<float>(test_img.cols / 2.), static_cast<float>(test_img.rows / 2.));
//    cv::Mat rot_mat = cv::getRotationMatrix2D(center, 15, 1.0);
//
//    Mat dst;
//    cv::Size dst_sz(test_img.cols, test_img.rows);
//    cv::warpAffine(test_img, dst, rot_mat, dst_sz, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
//    imshow("dst",dst);


/*    Mat test_img_gray;
    cvtColor(test_img, test_img_gray, COLOR_BGR2GRAY);
    vector<Vec3f> circles_test ; // 定位点
//    imshow("test_img",test_img_gray);

    HoughCircles(test_img_gray , circles_test , HOUGH_GRADIENT, 1, 10, 100,30, 1, 30);
        cout << circles_test[0] <<endl;*/








//    imshow("img_org",img_org);
//    imshow("Circles detected", greyimage);
    waitKey(0);
    return 1;

}
#endif

//==================================

//==================================

/*
 脚点旋转
 */


#if 001

void sort_point(){

}

double get_theta(Vec2f A, Vec2f B){
    double AB = A.dot(B);
//    double AB = A[0]*B[0]+A[1]*B[1];
    double AB_mor = hypot(A[0],A[1])* hypot(B[0],B[1]);
    double theta = acos(AB/AB_mor);
    return theta/M_PI*180.0;
};

double r_im(double value, double target = 90, double miss=10){
    if (value > target +miss || value < target-miss){
        return false;
    }
    return true;
}



int main (int argc, char* argv[]){
//    Mat input_img = imread("./images/Darwin_rotated.jpg",1);
    Mat input_img = imread("./images/Darwin_rotated_scaled.jpg",1);
    Mat greyimage, binary_img,adjust_img, dst_img;
    int th1 = 50,th2=150,maxLineGap=10;

    Canny(input_img, binary_img, 50,100,3);

    vector<Vec4i> plines;
    HoughLinesP(binary_img, plines, 1, CV_PI/180,th1,th2,maxLineGap);
    Scalar color = Scalar(0, 0, 255);

    input_img.copyTo(adjust_img);

    double degree;
    float k =(float) ( plines[0][3]- plines[0][1]) /( plines[0][2]- plines[0][0]);
    degree =atan(k)*180/M_PI;


    cv::Point2f center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, degree, 1.0);
    cv::warpAffine(input_img, adjust_img, rot_mat, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);

//   todo 根据脚点检测 确定 ;

    Mat empty_img = imread("./images/2Dempty.jpg",1);
//    vector<Vec3f> empty_img_point_location= get_point_locations(empty_img);
    vector<Vec3f> adjust_img_point_location= get_point_locations(adjust_img);
//    vector<Vec3f> adjust_img_point_location= get_point_locations(empty_img);
    imshow("empty_img",empty_img);
// 点排序


   // 左上：p2; 左下:p1 ; 右下：p3
//    Vec2f
    Point2f left_bot,
    left_top , right_bot,
    right_top ,temp;
    float ln= 0.0f;
//[1148.5, 1153.5, 19.6];[249.5, 247.5, 19.6];[1149.5, 247.5, 27.2];
    //确定 长度
    cout <<"abs:" << fabs(100-999) <<endl;
    for(Vec3f i : adjust_img_point_location){
        if (i[0]< i[1] && i[1] - i[0] >i[2] ){
            // 左下
            left_bot = Point2f(i[0],i[1]);
            ln= i[1]- i[0];
        }
        else if(i[0]> i[1] && i[0]-i[1] >i[2] ){
            // 右上
            right_top = Point2f(i[0],i[1]);
            ln= i[0]-i[1];
        }else{
            ln= fabs(i[1]-i[0]);
        }
    }

    for(Vec3f i : adjust_img_point_location){
         if(i[1] < ln  && fabs(i[1] - i[0]) <i[2]){
            // 左上
            left_top = Point2f(i[0],i[1]);
        }
        else if(i[1] > ln   && fabs(i[1] - i[0]) <i[2]){
            // 左上
            right_bot = Point2f(i[0],i[1]);
        }
    }


 /*   if(temp.x>left_bot.x  && temp.y > right_top.y   &&  temp.x> left_top.x &&  temp.y> left_top.y ){
        right_bot =  temp;
    }*/

    cout << "left_bot:"<<left_bot <<endl;
    cout << "left_top:"<<left_top <<endl;
    cout << "right_bot:"<<right_bot <<endl;
    cout << "right_top:"<<right_top <<endl;

//    cout << "temp:"<<temp <<endl;

/*    Point2f adjust_points[3];
    Point2f p2 = Point2f(adjust_img_point_location[0][0],adjust_img_point_location[0][1]);
    Point2f p1 = Point2f(adjust_img_point_location[1][0],adjust_img_point_location[1][1]);
    Point2f p3 = Point2f(adjust_img_point_location[2][0],adjust_img_point_location[2][1]);

    Vec2f v1_2 = Vec2f(p2.x-p1.x,p2.y-p1.y);
    Vec2f v1_3 = Vec2f(p3.x-p1.x , p3.y-p1.y);

    double theta = get_theta(v1_2,v1_3);

    int rotate = 0; // 不翻转
    if (p1.y > p2.y && p1.x < p3.x && r_im(get_theta(v1_2,v1_3),90) ){
        // 正图
        cout << "正图"<<endl;
    }
    else if (p1.y > p2.y && p1.x < p3.x && r_im(get_theta(v1_2,v1_3),45) ){
        // y 周翻转
        rotate = 2;
        cout << "y 周翻转"<<endl;
    }*/






//    cout <<"theta:"<< theta<<endl;
/*
    if (r_im(get_theta(v1_2,v1_3)), 90 ){
        // p1 点为直角
        if( (v1_2[1] < 0 && v1_3[0] > 0) || (v1_2[0] < 0 && v1_3[1] > 0) || (v1_2[0] > 0 && v1_3[1] > 0) || (v1_2[1] > 0 && v1_2[0] < 0) ){
            adjust_points[0]=p1;
            adjust_points[1]=p2;
            adjust_points[2]=p3;
        }
        else if( (v1_2[0] > 0 && v1_3[1] < 0) || (v1_2[1] < 0 && v1_3[0] < 0) ||  (v1_2[1] > 0 && v1_3[0] > 0) || (v1_2[0] < 0 && v1_3[1] > 0) ){
            adjust_points[0]=p1;
            adjust_points[1]=p3;
            adjust_points[2]=p2;
        }
    }
    else if (r_im(get_theta(v1_2,v1_3)),45){
        // p1 点为不是直角
        // p1
        if (hypot(v1_2[0],v1_2[1]) < hypot(v1_3[0], v1_3[1]) ){
           // p2 为直角
           //todo
        }
        else{
            //p3为直角
            //todo
        }
    }
    else{
        cout << "error with 3 point"<<endl;
        return -1;
    }
*/





    int rotate = 1; //1 x 轴点到
//    int rotate = 2; //2 y 轴点到
    Mat map_x, map_y;
    map_x.create(adjust_img.size(), CV_32FC1);
    map_y.create(adjust_img.size(), CV_32FC1);
    if(rotate == 1){
        for (int i = 0; i < adjust_img.rows; i++)
        {
            for (int j = 0; j < adjust_img.cols; j++)
            {
                map_x.at<float>(i, j) = static_cast<float>(j);//列不变
                map_y.at<float>(i, j) = static_cast<float>(adjust_img.rows - i);//行相反
            }
        }
    }
    else if(rotate == 2){
        for (int i = 0; i < adjust_img.rows; i++)
        {
            for (int j = 0; j < adjust_img.cols; j++)
            {
                map_x.at<float>(i, j) = static_cast<float>(i);
                map_y.at<float>(i, j) = static_cast<float>(adjust_img.cols - j);
            }
        }
    }
    remap(adjust_img, dst_img, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar::all(0));



//    imshow("input_img",input_img);
//    imshow("mid",mid);
    imshow("dst",adjust_img);

//    imshow("dstnewImage", dst_img);

    waitKey(0);
    return 1;

}
#endif
//==================================

/*
 直边旋转 done
 */


#if 000
int main (int argc, char* argv[]){
//    Mat input_img = imread("./images/Darwin_rotated.jpg",1);
    Mat input_img = imread("./images/Darwin_rotated_scaled.jpg",1);
    Mat greyimage, binary_img,adjust_img, dst_img;
    int th1 = 50,th2=150,maxLineGap=10;

    Canny(input_img, binary_img, 50,100,3);

    vector<Vec4i> plines;
    HoughLinesP(binary_img, plines, 1, CV_PI/180,th1,th2,maxLineGap);
    Scalar color = Scalar(0, 0, 255);

    input_img.copyTo(adjust_img);

    double degree;
    /*float k =(float) ( plines[0][3]- plines[0][1]) /( plines[0][2]- plines[0][0]);
    degree =atan(k)*180/M_PI;*/
    for (int i = 0; i <  plines.size(); i++) {
        Vec4i ln = plines[i];
        line(adjust_img, Point(ln[0],ln[1]), Point(ln[2],ln[3]), color,2,1 );
        float k =(float) (ln[3]-ln[1]) /(ln[2]-ln[0]);
        double answer = atan(k);
//        cout<<answer*180/M_PI<<endl;
        degree = answer*180/M_PI;
    }

    cv::Point2f center(static_cast<float>(input_img.cols / 2.), static_cast<float>(input_img.rows / 2.));
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, degree, 1.0);
    cv::warpAffine(input_img, adjust_img, rot_mat, Size(input_img.cols, input_img.rows), cv::INTER_LINEAR, cv::BORDER_REPLICATE);

//   todo 根据脚点检测 确定 ;

/*
    Mat empty_img = imread("./images/2Dempty.jpg",1);
    vector<Vec3f> empty_img_point_location= get_point_locations(empty_img);
    vector<Vec3f> adjust_img_point_location= get_point_locations(adjust_img);
        imshow("empty_img",empty_img);
*/




//    int rotate = 1; //1 x 轴点到
    int rotate = 2; //2 y 轴点到
    Mat map_x, map_y;
    map_x.create(adjust_img.size(), CV_32FC1);
    map_y.create(adjust_img.size(), CV_32FC1);
    if(rotate == 1){
        for (int i = 0; i < adjust_img.rows; i++)
        {
            for (int j = 0; j < adjust_img.cols; j++)
            {
                map_x.at<float>(i, j) = static_cast<float>(j);//列不变
                map_y.at<float>(i, j) = static_cast<float>(adjust_img.rows - i);//行相反
            }
        }
    }
    else if(rotate == 2){
        for (int i = 0; i < adjust_img.rows; i++)
        {
            for (int j = 0; j < adjust_img.cols; j++)
            {
                map_x.at<float>(i, j) = static_cast<float>(i);
                map_y.at<float>(i, j) = static_cast<float>(adjust_img.cols - j);
            }
        }
    }
    remap(adjust_img, dst_img, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar::all(0));



//    imshow("input_img",input_img);
//    imshow("mid",mid);
    imshow("dst",adjust_img);

//    imshow("dstnewImage", dst_img);

    waitKey(0);
    return 1;

}
#endif
//==================================


//==================================

/*
 原图检测
 */


#if 00
int main (int argc, char* argv[]){
//    Mat img_org = imread("./images/2Dempty.jpg",1);
    Mat img_org = imread("./images/Darwin_rotated.jpg",1);
    Mat greyimage ;
    medianBlur(img_org, img_org, 5);
    cvtColor(img_org, greyimage, COLOR_BGR2GRAY);
    vector<Vec3f> circles ; // 定位点
    HoughCircles(greyimage , circles , HOUGH_GRADIENT, 1, 30, 100,30, 1, 30);
    //画 定位点
    for( size_t i = 0; i < circles.size(); i++ ) {
        Vec3i c = circles[i];
        circle( img_org, Point(c[0], c[1]), c[2], Scalar (0,0,255) , 3, 1);
        cout << c <<endl;
    }
//    get_empty_point_locations();



//    imshow("img_org",img_org);
//    imshow("Circles detected", greyimage);
    waitKey(0);
    return 1;

}
#endif

/*
 encodingarray to bit
 */
#if 0
int convertBinaryToDecimal(long long n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}
int main (int argc, char* argv[]){
    for (int i = 0; i < sizeof(encodingarray); i++) {
        cout <<    encodingarray[i]<<"; ";

    }

    long long n;
    scanf("%lld", &n);

    printf("二进制数 %lld 转换为十进制为 %d", n, convertBinaryToDecimal(n));
    return 1;

}
#endif

//==================================

#if 0
int main (int argc, char* argv[]){
    for (int i = 0; i < sizeof(encodingarray); i++) {
        cout <<    encodingarray[i]<<"; ";

    }

    return 1;

}
#endif
//==================================

#if 0
int main (int argc, char* argv[]){
    Mat img = imread("./images/lena.jpg");
    Mat greyimage;
    cvtColor(img, greyimage, COLOR_BGR2GRAY);
    cout <<"a"<<endl;
    imshow("img",img);
    imshow("greyimage",greyimage);

    waitKey(0);
    return 1;

}
#endif





