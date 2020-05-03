//
// Created by Tommy on 2020-03-07.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]



/*
 * Hough transform
 * week4 Test3
 * 霍夫变换
 *
*/

#if 1

int th1 = 50,th2=150,maxLineGap=10;
void callback(int,void*){
    th1=getTrackbarPos("th1","params");
    th2=getTrackbarPos("th2","params");
}
int main(){
    VideoCapture capture;
    capture.open(0);

    namedWindow("params",0);
    createTrackbar("th1","params",&th1,500,callback);
    createTrackbar("th2","params",&th2,500,callback);
    createTrackbar("maxLineGap","params",&maxLineGap,100,callback);
    callback(0,0);

    Mat frame, mid, dst;

    while(1){
        capture >>frame;
        if(frame.empty()){break;}
        Canny(frame, mid, 50,100,3);
//        cvtColor(mid, mid, COLOR_GRAY2BGR);// TODO
        vector<Vec4i> plines;

        HoughLinesP(mid, plines, 1, CV_PI/180,th1,th2,maxLineGap);
//        lines = cvHoughLines2( dst , storage , HOUGHSTANDARD, 1,CVPI/180, 180);

//        CvMemStorage* storage = cvCreateMemStorage (0) ;
//        cvHoughLines2(  mid, void* line_storage, int method,
//                double rho, double theta, int threshold,
//                double param1=0, double param2=0 );
//        cout<< sizeof(lines)<<endl;

        Scalar color = Scalar(0, 0, 255);

        frame.copyTo(dst);

        for (int i = 0; i <  plines.size(); i++) {
            Vec4i ln = plines[i];

            line(dst, Point(ln[0],ln[1]), Point(ln[2],ln[3]), color,2,1 );
        }
        imshow("frame",frame);
        imshow("mid",mid);
        imshow("dst",dst);
        if(waitKey()==27){break;}
    }
    return 0;
}
#endif
//===================//

/*
 * edge detect
 * week4 Test2
 *
 * Canny
 * Sobel
 * Laplacian
 *
*/
#if 0
int th1 = 50,th2=50;
void callback(int,void*){
    th1=getTrackbarPos("th1","params");
    th2=getTrackbarPos("th2","params");
}
int main(){
    VideoCapture capture;
    capture.open(0);
    Mat frame , dst , furtherDst;
    namedWindow("params",0);
    createTrackbar("th1","params",&th1,500,callback);
    createTrackbar("th2","params",&th2,500,callback);
    callback(0,0);
    while(1){
        capture >>frame;
        if(frame.empty()){break;}

//        cvtColor(frame, frame, COLOR_BGR2GRAY);
//        Canny(frame, dst, th1,th2);
//        Sobel(frame, dst,-1, 1,1,3);
        Laplacian(frame, dst,-1,3);
//        cvLaplace(frame, dst,3);
//        convertScaleAbs( dst,furtherDst  ,1.5,10) ; //dst=abs(src*alpha+beta) //没啥卵用

        imshow("frame",frame);
        imshow("dst",dst);
//        imshow("furtherDst",furtherDst);
        if(waitKey()==27){break;}

    }
    return 0;
}
#endif

//===================//

/*
 * edge detect
 * week4 Test1
 *
 * LoG Laplacian of a Gaussian
 *
*/
#if 0

int main(){
    VideoCapture capture;
    capture.open(0);
    Mat frame , smooth, gray ,dst ;
    namedWindow("params",0);
    while(1){
        capture >>frame;
        if(frame.empty()){break;}

        /*two 3x3 kernels*/
//        GaussianBlur(frame,smooth,Size(3,3),0,0);
//        cvtColor(smooth, gray, COLOR_BGR2GRAY);
//        Laplacian(gray, dst,-1,3);

        /*a single 5x5 kernel*/
//        GaussianBlur(frame,smooth,Size(3,3),0,0);
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        Laplacian(gray, dst,-1,5);


        imshow("frame",frame);
        imshow("dst",dst);
        if(waitKey()==27){
            break;
        }

    }
    return 0;
}
#endif



//===================//

#if 0
/*
 * Not working
 */
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
// filters with track bar
#if 0
int g_blurValue = 5;
Mat boxImage,blurImage;
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
/*
 *   boxFilter
 */
int main(int argc, const char * argv[]) {
    Mat img = imread("./images/lena.jpg",0);
    Mat boxImage,blurImage,gaussImage;

    boxFilter(img,boxImage,-1,Size(3,3)); //方框滤波 当normalize=true为均值滤波
//    blur(img,blurImage, Size(7,7));
    blur(img,blurImage,Size(3,3)); // 均值滤波
    GaussianBlur(img,gaussImage,Size(3,3),0,0);

    imshow("img1",img);
    imshow("boxImage",boxImage);
    imshow("blurImage",blurImage);
//    imshow("gaussImage",gaussImage);
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