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

int main (){
    int kernel =7;
    Mat image_out;
    Mat src =  imread("./images/lena.jpg",0);
    src.copyTo(image_out);
    const int height = src.rows;
    const int width = src.cols;



}
#endif

//==================================
#if 0
void blur_demo(Mat &image, Mat &sum);
void edge_demo(Mat &image, Mat &sum);
int getblockSum(Mat &sum, int x1, int y1, int x2, int y2, int i);
int main(int argc, char** argv) {
    Mat src =  imread("./images/lena.jpg",0);

    if (src.empty()) {
        printf("could not load image...\n");
        return -1;
    }
//    namedWindow("input", CV_WINDOW_AUTOSIZE);
    imshow("input", src);
//    namedWindow("output", CV_WINDOW_AUTOSIZE);
    // �������ͼ
    Mat sum, sqrsum;
    integral(src, sum, sqrsum, CV_32S, CV_32F);
    // ����ͼӦ��
    int type = 0;
    blur_demo(src, sum);

    while (true) {
        char c = waitKey(100);
        if (c > 0) {
            type = (int)c;
            printf("c : %d\n", type);
        }
        if (c == 27) {
            break; // ESC
        }
        if (type == 49) { // ���ּ� 1
            blur_demo(src, sum);
        }
        else if (type == 50) { // ���ּ� 2
            edge_demo(src, sum);
        }
        else {
            blur_demo(src, sum);
        }
    }
    waitKey(0);
    return 0;
}
void blur_demo(Mat &image, Mat &sum) {
    int w = image.cols;
    int h = image.rows;
    Mat result = Mat::zeros(image.size(), image.type());
    int x2 = 0, y2 = 0;
    int x1 = 0, y1 = 0;
    int ksize = 5;
    int radius = ksize / 2;
    int ch = image.channels();
    int cx = 0, cy = 0;
    for (int row = 0; row < h + radius; row++) {
        y2 = (row + 1)>h ? h : (row + 1);
        y1 = (row - ksize) < 0 ? 0 : (row - ksize);
        for (int col = 0; col < w + radius; col++) {
            x2 = (col + 1)>w ? w : (col + 1);
            x1 = (col - ksize) < 0 ? 0 : (col - ksize);
            cx = (col - radius) < 0 ? 0 : col - radius;
            cy = (row - radius) < 0 ? 0 : row - radius;
            int num = (x2 - x1)*(y2 - y1);
            for (int i = 0; i < ch; i++) {
                // ����ͼ���Һͱ����������
                int s = getblockSum(sum, x1, y1, x2, y2, i);
                result.at<Vec3b>(cy, cx)[i] = saturate_cast<uchar>(s / num);
            }
        }
    }
    imshow("output", result);
    imwrite("./result.png", result);
}
/**
* 3x3 sobel ��ֱ��Ե�����ʾ
*/
void edge_demo(Mat &image, Mat &sum) {
    int w = image.cols;
    int h = image.rows;
    Mat result = Mat::zeros(image.size(), CV_32SC3);
    int x2 = 0, y2 = 0;
    int x1 = 0, y1 = 0;
    int ksize = 3; // ���Ӵ�С�������޸ģ�Խ���ԵЧӦԽ����
    int radius = ksize / 2;
    int ch = image.channels();
    int cx = 0, cy = 0;
    for (int row = 0; row < h + radius; row++) {
        y2 = (row + 1)>h ? h : (row + 1);
        y1 = (row - ksize) < 0 ? 0 : (row - ksize);
        for (int col = 0; col < w + radius; col++) {
            x2 = (col + 1)>w ? w : (col + 1);
            x1 = (col - ksize) < 0 ? 0 : (col - ksize);
            cx = (col - radius) < 0 ? 0 : col - radius;
            cy = (row - radius) < 0 ? 0 : row - radius;
            int num = (x2 - x1)*(y2 - y1);
            for (int i = 0; i < ch; i++) {
                // ����ͼ���Һͱ����������
                int s1 = getblockSum(sum, x1, y1, cx, y2, i);
                int s2 = getblockSum(sum, cx, y1, x2, y2, i);
                result.at<Vec3i>(cy, cx)[i] = saturate_cast<int>(s2 - s1);
            }
        }
    }
    Mat dst, gray;
    convertScaleAbs(result, dst);
    normalize(dst, dst, 0, 255, NORM_MINMAX);
    cvtColor(dst, gray, COLOR_BGR2GRAY);
    imshow("output", gray);
    imwrite("D:/edge_result.png", gray);
}
int getblockSum(Mat &sum, int x1, int y1, int x2, int y2, int i) {
    int tl = sum.at<Vec3i>(y1, x1)[i];
    int tr = sum.at<Vec3i>(y2, x1)[i];
    int bl = sum.at<Vec3i>(y1, x2)[i];
    int br = sum.at<Vec3i>(y2, x2)[i];
    int s = (br - bl - tr + tl);
    return s;
}




#endif




/*
 * kuwahara filter
 *
 * */
#if 0

Mat image_in;
Mat image_out;
//积分数组
double my_sum[4096][4096];//最大支持4096*4096分辨率的图像
long double my_sqsum[4096][4096];

int get_mean(int h, int w, int neibourhood,int num)
{
    int val;
    switch (num)
    {
        case 1: val = (int)(my_sum[h+1][w+1] - my_sum[h - neibourhood+1][w+1] - my_sum[h+1][w - neibourhood+1] + my_sum[h - neibourhood+1][w - neibourhood+1]) / (neibourhood*neibourhood); break;
        case 2: val = (int)(my_sum[h+1][w + neibourhood] - my_sum[h+1][w] - my_sum[h - neibourhood+1][w + neibourhood] + my_sum[h - neibourhood+1][w]) / (neibourhood*neibourhood); break;
        case 3: val = (int)(my_sum[h + neibourhood][w+1] - my_sum[h][w+1] - my_sum[h + neibourhood][w - neibourhood+1] + my_sum[h][w - neibourhood+1]) / (neibourhood*neibourhood); break;
        case 4: val = (int)(my_sum[h + neibourhood][w + neibourhood] - my_sum[h + neibourhood][w] - my_sum[h][w + neibourhood] + my_sum[h][w ]) / (neibourhood*neibourhood); break;

    }
    return val;
}


int main (int argc, char* argv[]){
    int kernel =7;
    image_in = imread("./images/limes.tif",0);
    image_in.copyTo(image_out);
    //Mat image_in(10, 10, CV_8U);

    const int height = image_in.rows;
    const int width = image_in.cols;



    if (kernel<3||kernel>15)
    {
        throw runtime_error("the kernel is out of range");
        return -1;
    }
    if (kernel/2==0)
    {
        throw runtime_error("the kernel must be odd");
        return -2;
    }

    //计算积分数组
    for (int h = 0; h < image_in.rows; h++)
    {
        //得到第h行的首地址
        uchar* data = image_in.ptr<uchar>(h);
//        Mat sumii, sqsumii;
//        integral(image_in,my_sum,my_sqsum,CV_32F); //计算积分图sumii
//        integral(image,sumii,sumii,CV_32F); //计算积分图sumii

        for (int w = 0; w < image_in.cols; w++)
        {
            my_sum[h+1][w+1] = data[w] + my_sum[h][w+1] + my_sum[h+1][w] - my_sum[h][w];
            //??
        }
    }


    for (int h = 0; h < image_in.rows; h++)
    {
        //得到第h行的首地址
        uchar* data = image_in.ptr<uchar>(h);
        for (int w = 0; w < image_in.cols; w++)
        {
            my_sqsum[h+1][w+1] = data[w] * data[w] + my_sqsum[h + 1][w] + my_sqsum[h][w + 1] - my_sqsum[h][w];
        }
    }

    //kuwahara滤波器
    int neibourhood = kernel / 2 + 1;
    float N = neibourhood*neibourhood;
    for (int h = neibourhood; h < height-neibourhood;h++)
    {
        for (int w = neibourhood; w < width-neibourhood;w++)
        {
            //左上
            double L_T_sqsum = my_sqsum[h+1][w+1] - my_sqsum[h - neibourhood+1][w+1] - my_sqsum[h+1 ][w - neibourhood+1] + my_sqsum[h - neibourhood+1][w - neibourhood+1];
            double L_T_sum = my_sum[h+1][w+1] - my_sum[h - neibourhood+1][w+1] - my_sum[h+1][w - neibourhood+1] + my_sum[h - neibourhood+1][w - neibourhood+1];
            double left_top = L_T_sqsum - L_T_sum*L_T_sum / N;

            //右上
            double R_T_sqsum = my_sqsum[h+1][w  + neibourhood] - my_sqsum[h+1][w] - my_sqsum[h - neibourhood+1][w + neibourhood] + my_sqsum[h - neibourhood+1][w];
            double R_T_sum = my_sum[h+1][w + neibourhood] - my_sum[h+1][w] - my_sum[h - neibourhood+1][w + neibourhood] + my_sum[h - neibourhood+1][w];
            double right_top = R_T_sqsum - R_T_sum*R_T_sum / N;

            //left_bottom
            double L_B_sqsum = my_sqsum[h  + neibourhood][w+1] - my_sqsum[h][w+1] - my_sqsum[h + neibourhood][w - neibourhood+1] + my_sqsum[h][w - neibourhood+1];
            double L_B_sum = my_sum[h + neibourhood][w+1] - my_sum[h][w+1] - my_sum[h + neibourhood][w - neibourhood+1] + my_sum[h][w - neibourhood+1];
            double left_bottom = L_B_sqsum - L_B_sum*L_B_sum / N;

            //right_bottom
            double R_B_sqsum = my_sqsum[h + neibourhood][w + neibourhood] - my_sqsum[h][w  + neibourhood] - my_sqsum[h + neibourhood][w ] + my_sqsum[h ][w ];
            double R_B_sum = my_sum[h + neibourhood][w + neibourhood] - my_sum[h][w + neibourhood] - my_sum[h + neibourhood][w ] + my_sum[h ][w ];
            double right_bottom = R_B_sqsum - R_B_sum*R_B_sum / N;

            double min_var = (left_top > right_top) ? right_top : left_top;
            min_var = (left_bottom > min_var) ? min_var : left_bottom;
            min_var = (right_bottom > min_var) ? min_var : right_bottom;

            uchar kuwahara_val = 0;
            if (min_var == left_top)  kuwahara_val = get_mean(h, w,neibourhood, 1);
            else if (min_var == right_top) kuwahara_val=get_mean( h, w, neibourhood, 2);
            else if (min_var == left_bottom) kuwahara_val = get_mean(h, w, neibourhood, 3);
            else if(min_var==right_bottom)  kuwahara_val = get_mean( h, w, neibourhood, 4);

            if (kuwahara_val > 255)  kuwahara_val = 255;
            else if (kuwahara_val < 0) kuwahara_val = 0;

            uchar* point = image_out.ptr<uchar>(h);
            point[w] = kuwahara_val;
        }
    }

    namedWindow("original_image", 1);
    imshow("original_image", image_in);

    namedWindow("kuwahara", 1);
    imshow("kuwahara", image_out);

    waitKey(0);
    return 1;

    /*}
    else
        return 0;*/


}
#endif
//===================//



/*
 * Otsu’s Threshold
 * TODO
 *
 * */
#if 0
int main (int argc, char* argv[]){
    Mat img1 = imread("./images/akiyo1.jpg",0);
    Mat img2(img1.rows,img1.cols,CV_8U);
    double mean, var, sdev;
    int N = img1.rows*img1.cols;
    float c = 1.5;
    if(! img1.data) {cout <<"err..." <<endl;exit(0);}
    for (int x = 0; x < img1.cols; x++) {
        for (int y = 0; y < img1.rows; y++) {
            mean = mean + Mpixel(img1,x,y);
            var = var + pow(Mpixel(img1,x,y),2);
        }
    }
    mean = mean/N;
    var = (var/N)-(mean*mean);
    sdev = sqrt(var);
    for(int x=0; x<img1.cols;x++){ for ( int y=0; y<img1 . rows ; y++){
            float cpixel = (255.0*(Mpixel(img1 ,x,y) - mean + c*sdev))/(2*c*sdev);
            if (cpixel >255) cpixel=255;
            if (cpixel <0) cpixel=0;
            Mpixel(img2,x,y) = cvRound(cpixel);
        }
    }

    imshow("img1",img1);
    imshow("img2",img2);
    waitKey(0);

    return 0;
}
#endif
//===================//

/*
 * threshold 图像二值化
 *
 * */
#if 0
int main (int argc, char* argv[]){
//    if(argc!=2){ cout << "needs 1 argument, e.g. image.jpg" << endl ; exit (0) ;}
//    Mat img1 = imread(argv[1],0);
    Mat img1 = imread("./images/akiyo1.jpg",0);
    Mat img2(img1.rows,img1.cols,CV_8U);
    if(! img1.data) {cout <<"err..." <<endl;exit(0);}
    for (int x = 0; x < img1.cols; x++) {
        for (int y = 0; y < img1.rows; y++) {
            if(Mpixel(img1,x,y)>=128) {
                Mpixel(img2,x,y) = 255;
            }else{
                Mpixel(img2,x,y) = 0;
            }
        }
    }
    imshow("img1",img1);
    imshow("img2",img2);
    waitKey(0);

    return 0;
}
#endif
//===================//


/*
 * Negative image
 *
 * */
#if 1
int main (int argc, char* argv[]){
//    if(argc!=2){ cout << "needs 1 argument, e.g. image.jpg" << endl ; exit (0) ;}
//    Mat img1 = imread(argv[1],0);
    Mat img1 = imread("./images/akiyo1.jpg",0);
    Mat img2(img1.rows,img1.cols,CV_8U);
    if(! img1.data) {cout <<"err..." <<endl;exit(0);}
    for (int x = 0; x < img1.cols; x++) {
        for (int y = 0; y < img1.rows; y++) {
            Mpixel(img2,x,y) = 255-Mpixel(img1,x,y);
        }
    }
    imshow("img1",img1);
    imshow("img2",img2);
    waitKey(0);

    return 0;
}
#endif
//===================//
