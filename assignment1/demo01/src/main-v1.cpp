//
// Created by Tommy on 2020-03-07.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]


//==================================
/*
 kuwahara filter
 */
#if 1


int get_mean(int h, int w, int neibour,int num,Mat my_sum)
{
    int val;
    switch (num)
    {
        case 1: val = (int)(my_sum.at<float>(h+1,w+1) - my_sum.at<float>(h - neibour+1,w+1) - my_sum.at<float>(h+1,w - neibour+1) + my_sum.at<float>(h - neibour+1,w - neibour+1)) / (neibour*neibour); break;
        case 2: val = (int)(my_sum.at<float>(h+1,w + neibour) - my_sum.at<float>(h+1,w) - my_sum.at<float>(h - neibour+1,w + neibour) + my_sum.at<float>(h - neibour+1,w)) / (neibour*neibour); break;
        case 3: val = (int)(my_sum.at<float>(h + neibour,w+1) - my_sum.at<float>(h,w+1) - my_sum.at<float>(h + neibour,w - neibour+1) + my_sum.at<float>(h,w - neibour+1)) / (neibour*neibour); break;
        case 4: val = (int)(my_sum.at<float>(h + neibour,w + neibour) - my_sum.at<float>(h + neibour,w) - my_sum.at<float>(h,w + neibour) + my_sum.at<float>(h,w )) / (neibour*neibour); break;

    }
    return val;
}


int main (int argc, char* argv[]){
    Mat image_in, image_out;

    /*if (argc != 4) {printf("needs an input image\n");exit(0);}
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

    const int height = image_in.rows;
    const int width = image_in.cols;


    Mat my_sum = Mat::zeros(image_in.rows+1,image_in.cols+1, CV_32FC1);//最大支持4096*4096分辨率的图像
    Mat my_sqsum= Mat::zeros(image_in.rows+1,image_in.cols+1, CV_32FC1);


    //计算积分数组

    integral(image_in,my_sum,my_sqsum,CV_32F,CV_32F); //计算积分图sumii

//    cout << "image_in = " << endl << " " << image_in << endl << endl;
//    cout << "my_sum = " << endl << " " << my_sum << endl << endl;
//    cout << "my_sqsum = " << endl << " " << my_sqsum << endl << endl;



    //kuwahara滤波器
    int neibour = kernel / 2 + 1;
    float N = neibour*neibour;
    for (int h = neibour; h < height-neibour;h++)
    {
        for (int w = neibour; w < width-neibour;w++)
        {
/*    for (int h = neibour; h < 5;h++)
    {
        for (int w = neibour; w < 5;w++)
        {*/
            //左上
            double L_T_sqsum =  my_sqsum.at<float>(h+1,w+1) - my_sqsum.at<float>(h - neibour+1,w+1) - my_sqsum.at<float>(h+1,w - neibour+1) + my_sqsum.at<float>(h - neibour+1,w - neibour+1);
            double L_T_sum = my_sum.at<float>(h+1,w+1) - my_sum.at<float>(h - neibour+1,w+1) - my_sum.at<float>(h+1,w - neibour+1) + my_sum.at<float>(h - neibour+1,w - neibour+1);
            /*cout<<"+4:"<< my_sum.at<float>(h+1,w+1) <<endl;
            cout<<"-2:"<<  my_sum.at<float>(h - neibour+1,w+1)  <<endl;
            cout<<"-3:"<<my_sum.at<float>(h+1,w - neibour+1)  <<endl;
            cout<<"+1:"<< my_sum.at<float>(h - neibour+1,w - neibour+1) <<endl;
            cout<<"L_T_sum:"<<L_T_sum <<endl;*/
            double left_top =( L_T_sqsum - L_T_sum * L_T_sum / N)/N;
            // (sqsum - pow(sum,2)/n)/n
            //右上
            double R_T_sqsum = my_sqsum.at<float>(h+1,w  + neibour) - my_sqsum.at<float>(h+1,w) - my_sqsum.at<float>(h - neibour+1,w + neibour) + my_sqsum.at<float>(h - neibour+1,w);
            double R_T_sum = my_sum.at<float>(h+1,w + neibour) - my_sum.at<float>(h+1,w) - my_sum.at<float>(h - neibour+1,w + neibour) + my_sum.at<float>(h - neibour+1,w);
            double right_top =( R_T_sqsum - R_T_sum*R_T_sum / N)/N;
//            cout<<"right_top:"<<right_top <<endl;

            //left_bottom
            double L_B_sqsum = my_sqsum.at<float>(h  + neibour,w+1) - my_sqsum.at<float>(h,w+1) - my_sqsum.at<float>(h + neibour,w - neibour+1) + my_sqsum.at<float>(h,w - neibour+1);
            double L_B_sum = my_sum.at<float>(h + neibour,w+1) - my_sum.at<float>(h,w+1) - my_sum.at<float>(h + neibour,w - neibour+1) + my_sum.at<float>(h,w - neibour+1);

            double left_bottom =( L_B_sqsum - L_B_sum*L_B_sum / N)/N;
//            cout<<"left_bottom:"<<left_bottom <<endl;

            //right_bottom
            double R_B_sqsum = my_sqsum.at<float>(h + neibour, w + neibour) - my_sqsum.at<float>(h,w  + neibour) - my_sqsum.at<float>(h + neibour,w ) + my_sqsum.at<float>(h ,w );
            double R_B_sum = my_sum.at<float>(h + neibour,w + neibour) - my_sum.at<float>(h,w + neibour) - my_sum.at<float>(h + neibour,w ) + my_sum.at<float>(h ,w );


            double right_bottom =( R_B_sqsum - R_B_sum*R_B_sum / N)/N;
//            cout<<"right_bottom:"<<right_bottom <<endl;


            double min_var = (left_top > right_top) ? right_top : left_top;
            min_var = (left_bottom > min_var) ? min_var : left_bottom;
            min_var = (right_bottom > min_var) ? min_var : right_bottom;
                        cout<<"min_var:"<<min_var <<endl;



//            uchar kuwahara_val = 0;
            int kuwahara_val = 0;

            if(left_top > right_top){
               kuwahara_val= (int)R_T_sum / N;
            }else if (left_top < right_top){
                kuwahara_val =  (int)L_T_sum /N;
            }else if (left_bottom >left_top && left_bottom >right_top ){
                kuwahara_val =  (int)L_B_sum/N;
            }else{
                kuwahara_val =  (int)R_B_sum/N;
            }


            if (kuwahara_val > 255)  kuwahara_val = 255;
            else if (kuwahara_val < 0) kuwahara_val = 0;

            uchar* point = image_out.ptr<uchar>(h);
            point[w] = kuwahara_val;
        }
//        cout<<"============="<<endl;
    }


    namedWindow("original_image", 1);
    imshow("original_image", image_in);

    namedWindow("kuwahara", 1);
    imshow("kuwahara", image_out);

//        imwrite(argv [ 2 ],image_out);

    waitKey(0);
    return 1;

    /*}
    else
        return 0;*/


}
#endif






