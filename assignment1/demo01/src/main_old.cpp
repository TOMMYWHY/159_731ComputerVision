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
 * kuwahara filter
 *
 * */
#if 1

Mat image_in;
Mat image_out;
//积分数组
long double my_sum[4096][4096];//最大支持4096*4096分辨率的图像
long double my_sqsum[4096][4096];
//Mat my_sum;//最大支持4096*4096分辨率的图像
//Mat my_sqsum;

int get_mean(int h, int w, int neibour,int num)
{
    int val;
    switch (num)
    {
        case 1: val = (int)(my_sum[h+1][w+1] - my_sum[h - neibour+1][w+1] - my_sum[h+1][w - neibour+1] + my_sum[h - neibour+1][w - neibour+1]) / (neibour*neibour); break;
        case 2: val = (int)(my_sum[h+1][w + neibour] - my_sum[h+1][w] - my_sum[h - neibour+1][w + neibour] + my_sum[h - neibour+1][w]) / (neibour*neibour); break;
        case 3: val = (int)(my_sum[h + neibour][w+1] - my_sum[h][w+1] - my_sum[h + neibour][w - neibour+1] + my_sum[h][w - neibour+1]) / (neibour*neibour); break;
        case 4: val = (int)(my_sum[h + neibour][w + neibour] - my_sum[h + neibour][w] - my_sum[h][w + neibour] + my_sum[h][w ]) / (neibour*neibour); break;

    }
    return val;
}


int main (int argc, char* argv[]){
/*    if (argc != 4) {printf("needs an input image\n");exit(0);}
    image_in = imread ( argv [ 1 ] , 0) ;
    int kernel = atoi(argv[3]);*/

    int kernel =7;
    image_in = imread("./images/limes.tif",0);

    if ( (kernel<3||kernel>15) && (kernel/2==0) )
    {
        cout<<"Kernel should be 3 5 7 9 12 15" <<endl;
        exit(0);
    }

    image_in.copyTo(image_out);

    int height = image_in.rows;
    int width = image_in.cols;



//    integral(image_in,my_sum,my_sqsum,CV_32S,CV_32F); //计算积分图sumii

    //计算积分数组
    for (int h = 0; h < image_in.rows; h++)
    {
        //得到第h行的首地址
        uchar* data = image_in.ptr<uchar>(h);
//        Mat sumii, sqsumii;
//        integral(image_in,my_sum,my_sqsum,CV_32S,CV_32F); //计算积分图sumii
//        integral(image_in,sumii,sumii,CV_32F); //计算积分图sumii
        for (int w = 0; w < image_in.cols; w++)
        {
//            my_sum[h+1][w+1] = data[w] + my_sum[h][w+1] + my_sum[h+1][w] - my_sum[h][w];
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
    int neibour = kernel / 2 + 1;
    float N = neibour*neibour;
    for (int h = neibour; h < height-neibour;h++)
    {
        for (int w = neibour; w < width-neibour;w++)
        {
            //左上
            double L_T_sqsum = my_sqsum[h+1][w+1] - my_sqsum[h - neibour+1][w+1] - my_sqsum[h+1 ][w - neibour+1] + my_sqsum[h - neibour+1][w - neibour+1];
            double L_T_sum = my_sum[h+1][w+1] - my_sum[h - neibour+1][w+1] - my_sum[h+1][w - neibour+1] + my_sum[h - neibour+1][w - neibour+1];
            double left_top = L_T_sqsum - L_T_sum*L_T_sum / N;

            //右上
            double R_T_sqsum = my_sqsum[h+1][w  + neibour] - my_sqsum[h+1][w] - my_sqsum[h - neibour+1][w + neibour] + my_sqsum[h - neibour+1][w];
            double R_T_sum = my_sum[h+1][w + neibour] - my_sum[h+1][w] - my_sum[h - neibour+1][w + neibour] + my_sum[h - neibour+1][w];
            double right_top = R_T_sqsum - R_T_sum*R_T_sum / N;

            //left_bottom
            double L_B_sqsum = my_sqsum[h  + neibour][w+1] - my_sqsum[h][w+1] - my_sqsum[h + neibour][w - neibour+1] + my_sqsum[h][w - neibour+1];
            double L_B_sum = my_sum[h + neibour][w+1] - my_sum[h][w+1] - my_sum[h + neibour][w - neibour+1] + my_sum[h][w - neibour+1];
            double left_bottom = L_B_sqsum - L_B_sum*L_B_sum / N;

            //right_bottom
            double R_B_sqsum = my_sqsum[h + neibour][w + neibour] - my_sqsum[h][w  + neibour] - my_sqsum[h + neibour][w ] + my_sqsum[h ][w ];
            double R_B_sum = my_sum[h + neibour][w + neibour] - my_sum[h][w + neibour] - my_sum[h + neibour][w ] + my_sum[h ][w ];
            double right_bottom = R_B_sqsum - R_B_sum*R_B_sum / N;

            double min_var = (left_top > right_top) ? right_top : left_top;
            min_var = (left_bottom > min_var) ? min_var : left_bottom;
            min_var = (right_bottom > min_var) ? min_var : right_bottom;

            uchar kuwahara_val = 0;
            if (min_var == left_top)  kuwahara_val = get_mean(h, w,neibour, 1);
            else if (min_var == right_top) kuwahara_val=get_mean( h, w, neibour, 2);
            else if (min_var == left_bottom) kuwahara_val = get_mean(h, w, neibour, 3);
            else if(min_var==right_bottom)  kuwahara_val = get_mean( h, w, neibour, 4);

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

//    imwrite(argv [ 2 ],image_out);
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
#if 0
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
