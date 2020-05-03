#include <iostream>
//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <numeric>
#include <math.h>
//#include <cv.h>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ((uchar*)((image).data + (y)*(image.step)))[x]


/*
 * Watershed Segmentation
 * 分水岭分割
 * todo
*/

#if 0

int th1 = 50,th2=100;
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
    callback(0,0);

    Mat frame, mid, dst;

    while(1){
        capture >>frame;
        if(frame.empty()){break;}
        medianBlur(frame, mid, 5);
//        cvtColor(mid, mid, COLOR_GRAY2BGR);// TODO
        vector<Vec4i> lines;
        watershed(mid, lines, 1, CV_PI/180,th1,th2,10);
        /*frame.copyTo(dst);
        for (int i = 0; i < 20; i++) {
            Vec4i ln = lines[i];
            line(dst, Point(ln[0],ln[1]), Point(ln[2],ln[3]), Scalar(0,0,255),3,1 );
        }*/
        imshow("frame",frame);
        imshow("mid",mid);
//        imshow("dst",dst);
        if(waitKey()==27){break;}
    }
    return 0;
}
#endif




/*
 * edge detect
 * 重点两个阈值的选择
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
    Mat frame , dst;
    namedWindow("params",0);
    createTrackbar("th1","params",&th1,500,callback);
    createTrackbar("th2","params",&th2,500,callback);
    callback(0,0);
    while(1){
        capture >>frame;
        if(frame.empty()){break;}

        cvtColor(frame, frame, COLOR_BGR2GRAY);
        Canny(frame, dst, th1,th2);

        imshow("frame",frame);
        imshow("dst",dst);
        if(waitKey()=='q'){
            break;
        }

    }
    return 0;
}
#endif



/*
 * threshold
*/
#if 0

int th= 180; // 阈值 threshold
Mat  dst;
Mat img = imread("./images/Lena.jpg",1);
void threshold_callback(int,void*){
//    th = getTrackbarPos("th","threshold");
    threshold(img,dst,th,255,0);
    imshow("dst",dst);
}
int main(){
    Mat kernel (5,5,CV_8U,Scalar(1));
    namedWindow("threshold",0);
    createTrackbar("th","threshold",&th,255,threshold_callback);
    threshold_callback(th,0);

    imshow("frame",img);
    cv::waitKey(0);
    return 0;
}
#endif

/*
 * erode  dilate
*/
#if 0
int main(){
    Mat img = imread("./images/Lena.jpg",1);
    Mat  erodedImg, dilatedImg;
    Mat kernel (5,5,CV_8U,Scalar(1));

    erode(img,erodedImg,kernel);
    dilate(img,dilatedImg,kernel);

    imshow("frame",img);
    imshow("erodedImg",erodedImg);
    imshow("dilatedImg",dilatedImg);
    cv::waitKey(0);

    return 0;
}
#endif

/*
 * filter
*/
#if 0
int main(){
//    Mat img = imread("./images/Lena.jpg",0);
    VideoCapture capture;
    capture.open(0);
    Mat frame , blurImage;
    while(1){
        capture >>frame;
        if(frame.empty()){
            break;
        }
        blur(frame,blurImage,Size(20,20));
        imshow("frame",frame);
        imshow("blurImage",blurImage);
        if(waitKey()=='q'){
            break;
        }

    }
    return 0;
}
#endif

/*
 * convert gray
*/
#if 0
int main(){
    Mat img = imread("./images/Lena.jpg",1);
    int width = img.rows;
    int height = img.cols;
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1);
    //dst = np.zeros((h,w,1), np.uint8)

    for (int j=0; j<height; j++){
        for (int i=0; i<width; i++){
            out.at<uchar>(j, i) = (int)((float)img.at<cv::Vec3b>(j,i)[0] * 0.0722
                    + (float)img.at<cv::Vec3b>(j,i)[1] * 0.7152
                    + (float)img.at<cv::Vec3b>(j,i)[2] * 0.2126);
        }
    }
    cv::imshow("answer", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
#endif
/*
 * convert color
*/
#if 0
int main(){
    Mat img = imread("./images/Lena.jpg",1);
    imshow("src",img);

    Mat gray,hsv,r_g_b;
    cvtColor(img,gray,COLOR_BGR2GRAY);

    imshow("gray",gray);

    cvtColor(img, hsv,COLOR_RGB2HSV);
    vector<Mat> hsvChannels;
    split(hsv,hsvChannels);
    imshow("hsv_h",hsvChannels[0]);
    imshow("hsv_s",hsvChannels[1]);
    imshow("hsv_v",hsvChannels[2]);
    imshow("hsv",hsv);

    waitKey(0);
    return 0;
}
#endif

#if 0
int main(){
    Mat img = imread("./images/Lena.jpg",0);
    imshow("src",img);
    waitKey(0);
    return 0;
}
#endif