//
// Created by Tommy on 2020-03-07.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

#include "camera_HSV_filter.c"
#include "camera_RGB_filter.c"

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x) * ( ( image ).channels() )]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x) * ( ( image ).channels() )+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x) * ( ( image ).channels() )+2]




/*Exercise 4
 * todo
*/
#if 1

int main(){
    VideoCapture capture;
    capture.open(0);
    Mat frame,dst ;

    while(1){
        capture >>frame;
        if(frame.empty()){break;}


        imshow("frame",frame);
//        imshow("mid",dst);
        if(waitKey()==27){break;}
    }
    return 0;
}
#endif

/*Exercise 3
 * Write a program that segments images by colour.
 * Use a range of values that is equivalent to a cubic region in the RGB space
 * (i.e., simple thresholds for each colour component).
 * Test your code using the images peppers.png and peppers2.png to try to separate the green peppers from the red ones.
 *
*/
#if 0

int main(){
//    Mat src = imread("./images/lena.jpg", 1);
    Mat src = imread("./images/peppers.bmp", 1);
    imshow("src",src);
    cout << src.channels() << endl;
    Mat dst = Mat(src.rows, src.cols,CV_8UC3,Scalar(0));


    for (int x = 0; x < dst.cols; x++) {
        for (int y = 0; y <dst.rows ; y++) {
            if(MpixelB(src,x,y)>100 && MpixelG(src,x,y)<100){
                MpixelR(dst,x,y)=255;
                MpixelG(dst,x,y)=0;
                MpixelB(dst,x,y)=0;
            }
             if (MpixelG(src ,x,y)>100 && MpixelR(src ,x,y)<100){
                MpixelR(dst ,x,y)=0;
                MpixelG(dst,x,y)=255;
                MpixelB(dst ,x,y)=0;
            }

            if (MpixelR(src ,x,y)>200 && MpixelG(src ,x,y)>200){
                MpixelR(dst,x,y)=255;
                MpixelG(dst,x,y)=255;
                MpixelB(dst ,x,y)=0;
            }
            if (MpixelR(src ,x,y)>240 && MpixelG(src ,x,y)<200){
                MpixelR(dst,x,y)=255;
                MpixelG(dst,x,y)=128;
                MpixelB(dst ,x,y)=0;
            }
        }
    }



    imshow("dst",dst);


    waitKey(0);
    return 0;
}
#endif

/*Exercise 2
 * Write a program that converts RGB to HSV and show the three components separately.
 * Remember that the range of the HSV space is different than that for RGB.
 * Adopt your own conversion table (e.g., for H, 0 is converted to 0, and 360 is converted to 255).
 *
*/
#if 0

int main(){
//    Mat src = imread("./images/lena.jpg", 1);
    Mat src = imread("./images/peppers.bmp", 1);
    imshow("src",src);
    Mat dst;
    cvtColor(src, dst,COLOR_BGR2HSV);
    imshow("dst",dst);


    waitKey(0);
    return 0;
}
#endif


/*
 * Exercise 1
 * Write a program that shows three greyscale images representing the three components R, G and B of a colour image.
 * (hint: just create three greyscale images and copy each component to one of them).
 *
*/
#if 0

int main(){
    Mat src = imread("./images/lena.jpg", 1);
//    Mat src = imread("./images/peppers.bmp", 1);
    imshow("src",src);
    vector<Mat> channels ;
    Mat dst_b,dst_g,dst_r ;
    split(src,channels);

    dst_b = channels[0].clone();
    dst_g =channels[1].clone();
    dst_r =channels[2].clone();

    imshow("dst_b",dst_b);
    imshow("dst_g",dst_g);
    imshow("dst_r",dst_r);

    waitKey(0);
    return 0;
}
#endif

/*
 * 3 channel split
 * */
#if 0
int main(){
    Mat src = imread("./images/lena.jpg", 1);
    imshow("src",src);

    vector<Mat> channels ;
    Mat  temp_channels[3],dst_b,dst_g,dst_r ;
    split(src,channels);

    temp_channels[0] = channels[0].clone();
    temp_channels[1] =channels[1].clone();
    temp_channels[2] =channels[2].clone();


    channels[0] = temp_channels[0];
    channels[1] =0;
    channels[2] =0;
    merge(channels,dst_b);
    imshow("dst_b",dst_b);

    channels[0] =0;
    channels[1] = temp_channels[1];
    channels[2] =0;
    merge(channels,dst_g);
    imshow("dst_g",dst_g);

    channels[0] =0;
    channels[1] =0;
    channels[2] = temp_channels[2];
    merge(channels,dst_r);
    imshow("dst_r",dst_r);

    waitKey(0);
    return 0;
}
#endif


//===================//

#if 0

int main(){
    Mat image_in = imread("./images/lena.jpg", 0);
    imshow("img1",image_in);

    if(waitKey()==27){break;}
    return 0;
}
#endif