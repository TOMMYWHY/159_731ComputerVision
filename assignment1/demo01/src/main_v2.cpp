//
// Created by Tommy on 2020-03-07.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include<algorithm>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]

//==================================
/*
 kuwahara filter
 */


#if 1
int main (int argc, char* argv[]){
    Mat image_in, image_out;

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
//    const int height = image_in.rows;
//    const int width = image_in.cols;

    Mat sumi,sqsumii;
    integral(image_in,sumi,sqsumii,CV_32F,CV_32F); // Integral Image

    //kuwahara filter
    int neibour = kernel / 2 + 1; // todo neibour to padding
    float N = neibour*neibour;
    for (int h = neibour; h < image_in.rows-neibour;h++)
    {
        for (int w = neibour; w < image_in.cols-neibour;w++)
        {
            //left_top
            double L_T_sqsum =  sqsumii.at<float>(h+1,w+1) - sqsumii.at<float>(h - neibour+1,w+1) - sqsumii.at<float>(h+1,w - neibour+1) + sqsumii.at<float>(h - neibour+1,w - neibour+1);

            double L_T_sum = sumi.at<float>(h+1,w+1) - sumi.at<float>(h - neibour+1,w+1) - sumi.at<float>(h+1,w - neibour+1) + sumi.at<float>(h - neibour+1,w - neibour+1);
//            double L_T_sum =Mpixel(sumi,h+1,w+1) - Mpixel(sumi,h - neibour+1,w+1) - Mpixel(sumi,h+1,w - neibour+1) +Mpixel( sumi, h - neibour+1,w - neibour+1);

            double left_top =( L_T_sqsum - L_T_sum * L_T_sum / N)/N;   // (sqsum - pow(sum,2)/n)/n

            //right_top
            double R_T_sqsum = sqsumii.at<float>(h+1,w  + neibour) - sqsumii.at<float>(h+1,w) - sqsumii.at<float>(h - neibour+1,w + neibour) + sqsumii.at<float>(h - neibour+1,w);
            double R_T_sum = sumi.at<float>(h+1,w + neibour) - sumi.at<float>(h+1,w) - sumi.at<float>(h - neibour+1,w + neibour) + sumi.at<float>(h - neibour+1,w);
            double right_top =( R_T_sqsum - R_T_sum*R_T_sum / N)/N;

            //left_bottom
            double L_B_sqsum = sqsumii.at<float>(h  + neibour,w+1) - sqsumii.at<float>(h,w+1) - sqsumii.at<float>(h + neibour,w - neibour+1) + sqsumii.at<float>(h,w - neibour+1);
            double L_B_sum = sumi.at<float>(h + neibour,w+1) - sumi.at<float>(h,w+1) - sumi.at<float>(h + neibour,w - neibour+1) + sumi.at<float>(h,w - neibour+1);
            double left_bottom =( L_B_sqsum - L_B_sum*L_B_sum / N)/N;

            //right_bottom
            double R_B_sqsum = sqsumii.at<float>(h + neibour, w + neibour) - sqsumii.at<float>(h,w  + neibour) - sqsumii.at<float>(h + neibour,w ) + sqsumii.at<float>(h ,w );
            double R_B_sum = sumi.at<float>(h + neibour,w + neibour) - sumi.at<float>(h,w + neibour) - sumi.at<float>(h + neibour,w ) + sumi.at<float>(h ,w );
            double right_bottom =( R_B_sqsum - R_B_sum*R_B_sum / N)/N;

            double arr[] = {left_top,right_top,left_bottom,right_bottom};
            int minPosition = min_element(arr,arr+4) - arr;
            cout << "min value: " <<minPosition << endl;

            int kuwahara_val = 0;
            if(minPosition == 0 ){
               kuwahara_val= (int)L_T_sum/ N;
            }else if(minPosition == 1){
                kuwahara_val =  (int)R_T_sum /N;
            }else if (minPosition == 2){
                kuwahara_val =  (int)L_B_sum/N;
            }else if (minPosition == 3){
                kuwahara_val =  (int)R_B_sum/N;
            }

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




}
#endif





