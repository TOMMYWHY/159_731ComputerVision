//
//  main.cpp
//  opengcv_demo
//
//  Created by Tommy on 2020/3/20.
//  Copyright © 2020 Tommy. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;




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
    image_in = imread("./images/lena.jpg",0);
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

#if 1

int main(){

    Mat image=imread("./images/lena.jpg", 0);
    //cvtColor(image,image,CV_RGB2GRAY); //原图像是三通道，积分图也是三通道
    Mat sumii, sqsumii;
    integral(image,sumii,sqsumii,CV_32F); //计算积分图sumii
//    integral(image,sqsumii,sqsumii,CV_32F); //计算积分图sqsumii
    
    normalize(sumii,sumii,0,255,NORM_MINMAX);  //归一化，方便显示
    normalize(sqsumii,sqsumii,0,255,NORM_MINMAX);  //归一化，方便显示

    Mat imageIntegralNorm,sqsumiiNorm;
    convertScaleAbs(sumii,imageIntegralNorm); //精度转换为8位int整型
    convertScaleAbs(sqsumii,sqsumiiNorm); //精度转换为8位int整型

    imshow("Source Image",image);
    imshow("Integral Image",imageIntegralNorm);
    imshow("sqsumii",sqsumiiNorm);


    waitKey(0);
   return 0;
}
#endif

//===================//
/*
 filter
 */
#if 0
int g_blurValue = 5;
Mat boxImage,blurImage,gaussImage;
Mat img = imread("./images/lena.jpg",0);
static void on_Blur(int, void*){
    blur(img,boxImage,Size(g_blurValue+1,g_blurValue+1));
    imshow("on_Blur",boxImage);
};
int main(int argc, const char * argv[]) {
    namedWindow("on_Blur");
    createTrackbar("kernel","on_Blur",&g_blurValue,20,on_Blur);
    on_Blur(g_blurValue,0);
    imshow("org",img);
    waitKey(0);
    return 0;
}

#endif
  
