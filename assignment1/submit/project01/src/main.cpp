/*
// *  Assignment 2: kuwahara filter , 159.731, S1 2020
// *  Family Name: JIANG , Given Name: TAO , Student ID:20004769,
// *
// * */
#include <iostream>
#include <opencv2/opencv.hpp>
#include<algorithm>

using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]

#if 1
int main (int argc, char* argv[]){
    Mat src, output;
    namedWindow("src", WINDOW_AUTOSIZE);
    namedWindow("output", WINDOW_AUTOSIZE);

    if (argc != 4) {printf("needs an input image\n");exit(0);}
    src = imread ( argv [ 1 ] , 0) ;
    int kernel = atoi(argv[3]);

//    int kernel =15;
//    src = imread("./images/limes.tif",0);

    if ( (kernel<3||kernel>15) && (kernel/2==0) )
    { cout<<"Kernel should be 3 5 7 9 12 15" <<endl;exit(0);}
    src.copyTo(output);
    Mat sumi,sqsumii;
    integral(src,sumi,sqsumii,CV_32F,CV_32F);
    int nb = kernel / 2 + 1;
    float N = nb*nb;
    for (int h = nb; h < src.rows-nb;h++)
    {
        for (int w = nb; w < src.cols-nb;w++)
        {
            double l_t_sum = sumi.at<float>(h+1,w+1) - sumi.at<float>(h - nb+1,w+1) - sumi.at<float>(h+1,w - nb+1) + sumi.at<float>(h - nb+1,w - nb+1);
            double l_t_sqsum =  sqsumii.at<float>(h+1,w+1) - sqsumii.at<float>(h - nb+1,w+1) - sqsumii.at<float>(h+1,w - nb+1) + sqsumii.at<float>(h - nb+1,w - nb+1);

            double t_t_sum = sumi.at<float>(h+1,w + nb) - sumi.at<float>(h+1,w) - sumi.at<float>(h - nb+1,w + nb) + sumi.at<float>(h - nb+1,w);
            double r_t_sqsum = sqsumii.at<float>(h+1,w  + nb) - sqsumii.at<float>(h+1,w) - sqsumii.at<float>(h - nb+1,w + nb) + sqsumii.at<float>(h - nb+1,w);

            double l_b_sum = sumi.at<float>(h + nb,w+1) - sumi.at<float>(h,w+1) - sumi.at<float>(h + nb,w - nb+1) + sumi.at<float>(h,w - nb+1);
            double l_b_sqsum = sqsumii.at<float>(h  + nb,w+1) - sqsumii.at<float>(h,w+1) - sqsumii.at<float>(h + nb,w - nb+1) + sqsumii.at<float>(h,w - nb+1);

            double r_b_sum = sumi.at<float>(h + nb,w + nb) - sumi.at<float>(h,w + nb) - sumi.at<float>(h + nb,w ) + sumi.at<float>(h ,w );
            double r_b_sqsum = sqsumii.at<float>(h + nb, w + nb) - sqsumii.at<float>(h,w  + nb) - sqsumii.at<float>(h + nb,w ) + sqsumii.at<float>(h ,w );

            double left_top =( l_t_sqsum - l_t_sum * l_t_sum / N)/N;
            double right_top =( r_t_sqsum - t_t_sum*t_t_sum / N)/N;
            double left_bottom =( l_b_sqsum - l_b_sum*l_b_sum / N)/N;
            double right_bottom =( r_b_sqsum - r_b_sum*r_b_sum / N)/N;

            double arr[] = {left_top,right_top,left_bottom,right_bottom};
            int minPosition = min_element(arr,arr+4) - arr;
//            cout << " minPosition: " <<minPosition << endl;
            int kuwahara_val = 0;
            if(minPosition == 0 ){
               kuwahara_val= (int)l_t_sum/ N;
            }else if(minPosition == 1){
                kuwahara_val =  (int)t_t_sum /N;
            }else if (minPosition == 2){
                kuwahara_val =  (int)l_b_sum/N;
            }else if (minPosition == 3){
                kuwahara_val =  (int)r_b_sum/N;
            }
//            cout  <<kuwahara_val << endl;
//            point[w] = kuwahara_val;
            Mpixel(output,w - nb,h - nb) = kuwahara_val;
        }
    }
    imshow("src", src);
    imshow("output", output);

    imwrite(argv [ 2 ],output);

    waitKey(0);
    return 1;




}
#endif





