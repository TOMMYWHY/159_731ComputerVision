//
// Created by Tommy on 2020-03-07.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]


#if 0

int main(){
    int kernel =7;
    Mat image_in;
    Mat image_out;
    image_in = imread("./images/lena.jpg",0);

    Mat my_sum= Mat::zeros(image_in.size(), image_in.type());//最大支持4096*4096分辨率的图像
    Mat my_sqsum = Mat::zeros(image_in.size(), image_in.type());
    integral(image_in,my_sum,my_sqsum,CV_32F); //计算积分图sumii
    cout << "M = " << endl << " " << my_sum << endl << endl;
    Mat theta;
//    theta =
//    cout <<my_sum[100,100] <<endl;
    waitKey(0);
    return 0;


}

#endif


//===================//
// filters
#if 0
int g_blurValue = 5;
Mat boxImage,blurImage,gaussImage;
Mat img = imread("./images/lena.jpg",0);
static void on_Blur(int, void*){
    blur(img,boxImage,Size(g_blurValue,g_blurValue));
    imshow("on_Blur",boxImage);
};
int main(int argc, const char * argv[]) {
    namedWindow("window1");
    createTrackbar("kernel","window1",&g_blurValue,20,on_Blur);
    on_Blur(g_blurValue,0);
    imshow("img1",img);

    waitKey(0);
    return 0;
}
#endif
//===================//


// filters
#if 0
int main(int argc, const char * argv[]) {
    Mat img = imread("./images/lena.jpg",0);
    Mat boxImage,blurImage,gaussImage;

    boxFilter(img,boxImage,-1,Size(3,3));
    blur(img,blurImage,Size(7,7));
    GaussianBlur(img,gaussImage,Size(3,3),0,0);

    imshow("img1",img);
    imshow("boxImage",boxImage);
    imshow("blurImage",blurImage);
    imshow("gaussImage",gaussImage);
    waitKey(0);
    return 0;
}
#endif


//===================//

#if 0

int main(){
    Mat image_in = imread("./images/lena.jpg", 0);
    return 0;
}
#endif