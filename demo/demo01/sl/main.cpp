//
// Created by Tommy on 2020-03-07.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <numeric>
#include <math.h>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ((uchar*)((image).data + (y)*(image.step)))[x]

/*
 * testing
*/
#if 0
int main(){
    Point2f center;
    center.x = 176/2;
    center.y = 176/2;

    Mat img = imread("./images/Lena.jpg",0);
    Mat rotmatrix=getRotationMatrix2D(center, 30, 1);
    cout<<"testing" <<endl;
    return 0;
}
#endif


/*
 * week3 Exercise 2
 * Write a program that smooths the image using a linear filter.
 * The program should allow the kernel for the linear filter to be created at different sizes
 * (take a parameter for the kernel size).
*/
#if 1
int main(){
//    Mat img = imread("./images/Lena.jpg",0);
    Mat img = imread("./images/peppers.bmp",0);
    Mat result = Mat(img.rows,img.cols,CV_8U,Scalar(0));
    /*越界为 0
     * */
//    cout << "-1:" << (int)Mpixel(img,-1,-1)<<endl;
//    cout << "1:" << (int)Mpixel(img,1,1)<<endl;

//    float kernel[9] = { 1.0 / 9.0, 1.0 / 9.0,1.0 / 9.0,
//                        1.0 / 9.0,1.0 / 9.0,1.0 / 9.0,
//                        1.0 / 9.0,1.0 / 9.0,1.0 / 9.0, };
//    float kernel[9] = {0,1,0,   1,-4,1,    0,1,0 };
    float kernel[9] = {-1,-2,-1,    0,0,0, 1,2,1 };
    float pos[9] = {0};
//    int x_boundary = img.cols-1,y_boundary=img.rows-1;
    int x_boundary = img.cols,y_boundary=img.rows;
    for (int y = 0; y < x_boundary; y++) {
        for (int x = 0; x < y_boundary; x++) {
            if(Mpixel(img,x-1,y-1) ==0 || Mpixel(img,x,y-1) || Mpixel(img,x+1,y-1)||
                    Mpixel(img,x-1,y) ==0 ||Mpixel(img,x+1,y) ==0 ||
                    Mpixel(img,x+1,y-1) ==0 ||Mpixel(img,x,y+1) ==0 ||Mpixel(img,x+1,y+1) ==0){
                Mpixel(result,x,y) = Mpixel(img,x,y);
            }
            pos[0] = Mpixel(img,x-1,y-1)*kernel[0];
            pos[1] = Mpixel(img,x,y-1)*kernel[1];
            pos[2] = Mpixel(img,x+1,y-1)*kernel[2];

            pos[3] = Mpixel(img,x-1,y)*kernel[3];
            pos[4] = Mpixel(img,x,y)*kernel[4];
            pos[5] = Mpixel(img,x+1,y)*kernel[5];

            pos[6] = Mpixel(img,x+1,y-1)*kernel[6];
            pos[7] = Mpixel(img,x,y+1)*kernel[7];
            pos[8] = Mpixel(img,x+1,y+1)*kernel[8];

            Mpixel(result,x,y) = accumulate(pos, pos + 9, 0);
        }
    }
    imshow("img",img);
    imshow("result",result);
    waitKey(0);
    return 0;
}
#endif

/* 3.3 read write
*
* */
#if 0
int main (int argc, char* argv[]){
    //gray img
    /*Mat img = imread("./images/Lena.jpg",0);
    MatIterator_ <uchar> grayit, grayend;
    for (grayit = img.begin<uchar>(),grayend=img.end<uchar>(); grayit!=grayend; grayit++) {
        *grayit = rand()%255;
    }*/

    // color img
    Mat img = imread("./images/Lena.jpg",1);
    MatIterator_ <Vec3b> grayit, grayend;
    for (grayit = img.begin<Vec3b>(),grayend=img.end<Vec3b>(); grayit!=grayend; grayit++) {
        (*grayit)[0] = rand()%255;
        (*grayit)[1] = rand()%255;
        (*grayit)[2] = rand()%255;
    }
    imshow("img",img);
    waitKey(0);
    return 0;
}
#endif
//===================//

/* 3.2 read write
 *  iterator
 * */
#if 0
int main (int argc, char* argv[]){
    //gray img
    /*Mat img = imread("./images/Lena.jpg",0);
    MatIterator_ <uchar> grayit, grayend;
    for (grayit = img.begin<uchar>(),grayend=img.end<uchar>(); grayit!=grayend; grayit++) {
        *grayit = rand()%255;
    }*/

    // color img
    Mat img = imread("./images/Lena.jpg",1);
    MatIterator_ <Vec3b> grayit, grayend;
    for (grayit = img.begin<Vec3b>(),grayend=img.end<Vec3b>(); grayit!=grayend; grayit++) {
        (*grayit)[0] = rand()%255;
        (*grayit)[1] = rand()%255;
        (*grayit)[2] = rand()%255;
    }
    imshow("img",img);
    waitKey(0);
    return 0;
}
#endif
//===================//

/* 3.1 read write
 * at
 * */
#if 0

int main (int argc, char* argv[]){

    Mat img = imread("./images/Lena.jpg",1);

    int x= 180,y=200;
    uchar value = img.at<uchar>(x,y);
    for (int x = 0; x < img.cols; x++) {
        for (int y = 0; y < img.rows; y++) {
//            grayim.at<uchar>(x,y) = (x+y)%255;

            Vec3b pixel;
            pixel[0] = x%255;
            pixel[1] = y%255;
            pixel[2] = 0;
            img.at<Vec3b>(x,y) = pixel;
        }
    }
    imshow("img",img);
    waitKey(0);
    return 0;
}
#endif
//===================//

/* 2
 * Mat roi
 * */
#if 0

int main (int argc, char* argv[]){

   Mat pImg = imread("./images/Lena.jpg",1);
   Rect rect(180,200,200,200);
   Mat roi = Mat(pImg,rect); //Mat::Mat(const Mat& m, const Rect& roi)

   Mat pImgRect = pImg.clone(); // deep copy
   rectangle(pImgRect,rect,Scalar(0,255,0),2); //rectangle()、椭圆ellipse() 、圆circle()
   imshow("org",pImgRect);
   imshow("roi",roi);
   waitKey(0);
    return 0;
}
#endif
//===================//

/* 1
 *basic
 * BGR
 * */
#if 0

int main (int argc, char* argv[]){

    Mat m(3,2,CV_8UC3,Scalar(0,0,255)); //scalar for init bgr
    cout <<"m : "<< m <<endl;
    cout <<"testing" <<endl;

    return 0;
}
#endif
//===================//
