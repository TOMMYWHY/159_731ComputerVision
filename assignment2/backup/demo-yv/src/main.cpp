//
// Created by Tommy on 2020-03-07.
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
/*
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
*/

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



//==================================

/*
 图像裁剪 todo
 */


#if 01
int main (int argc, char* argv[]){
//    Mat img_org = imread("./images/2Dempty.jpg",1);
    Mat img_org = imread("./images/abcde.jpg",1);
    Mat greyimage,mid ,dst,blurred;
    Canny(img_org, mid, 50,100,3);

    GaussianBlur(mid, blurred, cv::Size(7, 7), 0);


/*    cvtColor(img_org, greyimage, COLOR_BGR2GRAY);
    vector<Vec4i> plines;
    HoughLinesP(mid, plines, 1, CV_PI/180,50,150,10);
    Scalar color = Scalar(0, 0, 255);
    img_org.copyTo(dst);
    for (int i = 0; i <  plines.size(); i++) {
        Vec4i ln = plines[i];

        line(mid, Point(ln[0],ln[1]), Point(ln[2],ln[3]), color,2,1 );
    }*/

//    Rect ROI = boundingRect(blurred);
//    get_empty_point_locations();


    imshow("image", blurred);

    imshow("img_org",img_org);
    imshow("mid",mid);
//    imshow("dst",dst);
    print(mid);

    waitKey(0);
    return 1;

}
#endif

//==================================

/*
 * decode // todo 解码错误。
 */
#if 0000

int main (int argc, char* argv[]){
    Mat img = imread("./images/abcde.jpg");
    Mat mid ,temp;
    mid.create(img.size(),CV_8UC3);
//    int step = 0;// todo 计算色块步长
    int step = 20;// todo 计算色块步长
    Vec3b new_pixel_pre;
    for (int x=0;x<img.cols;x++){
        for ( int y=0;y<img . rows ; y++) {
            Vec3b pixel_image = img.at<Vec3b>(y, x);
            Vec3b pixel_image2 = img.at<Vec3b>(y, x);
//            int b = pixel_image[0];
//            int g = pixel_image[1];
//            int r = pixel_image[2];
            pixel_image2[0]=pixel_image[0] >128 ? 255:0;
            pixel_image2[1]=pixel_image[1] >128 ? 255:0;
            pixel_image2[2]=pixel_image[2] >128 ? 255:0;
            mid.at<Vec3b>(y,x)=pixel_image2; // 颜色校正

/*            if(pixel_image[0]!=255 && pixel_image[1]!=255 && pixel_image[2]!=255){
//                cout << "b:" << b << "; g:" << g << "; r:" << r << endl;
                b=b>128 ? 255:0;
                g=g>128 ? 255:0;
                r=r>128 ? 255:0;
                Vec3i new_pixel = Vec3i(b,g,r);
                cout << new_pixel << endl;
                mid.at<Vec3i>(y,x)=new_pixel;
            }*/
        }
    }

//    print(mid);
    string decode_img ; //todo decode image 有误
    for (int x=1;x<mid.cols;x = x+20){
        for ( int y=1;y<mid.rows ; y+=20) {
            Vec3b pixel_image = mid.at<Vec3b>(y, x);
            int b = pixel_image[0];
            int g = pixel_image[1];
            int r = pixel_image[2];
//            cout << "b:" <<b << "; g:" <<g  << "; r:" << r  << endl;
            b=b>128 ? 1:0;
            g=g>128 ? 1:0;
            r=r>128 ? 1:0;
//            cout << "b:" <<b << "; g:" <<g  << "; r:" << r  << endl;

            string s_b=to_string(b);
            string s_g=to_string(g);
            string s_r=to_string(r);
            string res = s_b+s_g+s_r;
//            cout << "res:"<< res <<endl;
            decode_img += res;
        }
    }

    cout << decode_img<<endl;
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
    cout << decode_to_char<< endl;




    imshow("img",img);
    imshow("mid",mid);

    waitKey(0);
    return 1;

}


#endif

//==================================

//==================================

/*
 脚点旋转 done
 */


#if 0000

int main (int argc, char* argv[]){
    Mat input_img = imread("./images/Darwin_rotated.jpg",1);
//    Mat input_img = imread("./images/Darwin_rotated_scaled.jpg",1);
//    Mat input_img = imread("./images/congratulations_rotated_scaled.jpg",1);

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
//    imshow("empty_img",empty_img);
// 点排序
   // 左上：p2; 左下:p1 ; 右下：p3
//    Vec2f
    Point2f left_bot,
    left_top , right_bot,
    right_top ,temp;
    float ln= 0.0f;
    //确定 长度
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


    cout << "left_bot:"<<left_bot <<endl;
    cout << "left_top:"<<left_top <<endl;
    cout << "right_bot:"<<right_bot <<endl;
    cout << "right_top:"<<right_top <<endl;

    int angle=0;
    if(right_top.x==0&&right_top.y==0){
        cout << "正图"<<endl;
    } else if (right_bot.x==0 && right_bot.y==0){
        cout << "转90"<<endl;
        angle=90;
    }else if (left_bot.x==0 && left_bot.y==0){
        cout << "转180"<<endl;
        angle=180;
    }else if (left_top.x==0 && left_top.y==0){
        cout << "转270"<<endl;
        angle=270;
    }

    Mat M = getRotationMatrix2D(center,angle,1);//计算旋转的仿射变换矩阵
    warpAffine(adjust_img,dst_img,M,Size(adjust_img.cols,adjust_img.rows));//仿射变换



    imshow("input_img",input_img);
//    imshow("mid",mid);
//    imshow("adjust_img",adjust_img);

    imshow("dst_img", dst_img);

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
 *encodingarray to  bit
 * todo
 */
#if 00000


int main (int argc, char* argv[]){
    for (int i = 0; i < sizeof(encodingarray); i++) {
//        cout <<    encodingarray[i]<<"; ";
    }
//    string str = "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111010101100110000100111011011001101010001000110011010001100010000000111101011110101100110111010101100110111111111111111111111111111111111111100111111110010010000100011000001111000011111101101001110110100010111100101000110111000011011001001110010010000100111111111100111111111111111111111111100111111111110101010110100100000111111001011010101000001011110001010010100000000011111110011000001111110101010110111111111100111111111111111111100100111100100001010110000010011100001000010111111011101101110001100110111010101100110000100111011101001001010110000010111100100111100100111111111111111111100100111111001101110110010100100111000101111010011000101011001001110010010000100011000001111100101111001101110110111111100100100111111111111111111111100111111101010001000110011010001100010000000111101011110101100001111110101010110100100000111011001101010001000110111111111100111111111111111000001101101010011111101101001110110100010111100101000110111000011011101001001010110000010011100001000100011111101101001110000000110001011100111111111000010100111010001011010101000001011110001010010100000000011111110011100101111001101110110010100100111111001011010101000001011010010101011000111111111000101101011010111111011101101110001100110111010101100110000100111011011001101010001000110011010001100010111111011101101110100000001001111100111111111100010010111110101111010011000101011001001110010010000100011000001111000011111"; //dc
    string str = "010011000010101101011100"; //s
//    string temp;
//    cout <<"000100000011 dc:" <<   str.size()<<"; "<<endl;
    string result;
    for (int i = 0; i < str.size(); i=i+6) {
        if (i%6==0){
            string sigle = str.substr(i, 6);
//            cout <<"sigle: " <<   sigle<<"; "<<endl;
//            cout << sigle<<endl;

            reverse(sigle.begin(),sigle.end()); // 翻转字符串
            const char* arr = sigle.data();
            int ret = convertBinaryToDecimal(arr); // 二进制从后往前读
//            cout << encodingarray[ret] << endl;
//            cout << ret << endl;
            cout << sigle<<"=>"<< ret <<"  ;  " <<endl;
            result +=encodingarray[ret];
        }
    }

//    string str1 = "000100";
//    reverse(str1.begin(),str1.end()); // 翻转字符串
//    const char* arr = str1.data();
//    int ret = convertBinaryToDecimal(arr); // 二进制从后往前读
//
//    printf("ret = %d\n", ret);
    cout << result<< endl;
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





