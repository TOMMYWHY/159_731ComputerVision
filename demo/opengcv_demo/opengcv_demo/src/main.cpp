//
//  main.cpp
//  opengcv_demo
//
//  Created by Tommy on 2020/3/20.
//  Copyright © 2020 Tommy. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
using namespace std;
using namespace cv;


//==================================
/*
 read video
 */
#if 1

int main (){
   
    VideoCapture cap("./images/demo.mp4");
    while (true) {
        Mat frame,output;
        cap >> frame;
        if(frame.empty())break;
        resize(frame,output, Size(480,360));
        imshow("output", output);
       if(waitKey(1) >= 0) break; // 帧速
    }
        cout <<"testing"<<endl;
    return 0;
}

#endif
//==================================


/*
 read camera
 */
#if 0

int main (){
   
    VideoCapture cap(0);
    while (true) {
        Mat frame,output;
        cap >> frame;
        resize(frame,output, Size(480,360));
        imshow("output", output);
       if(waitKey(30) >= 0) break;
    }
        cout <<"testing"<<endl;
    return 0;
}

#endif





//==================================
/*
 kuwahara filter
 */
#if 0
Mat image_in =imread("./images/limes.tif",0);
//Mat image_in =imread("./images/test.jpeg",0);
Mat image_out;
//积分数组
//double my_sum[4096][4096];//最大支持4096*4096分辨率的图像
//long double my_sqsum[4096][4096];

int kernel =7;
//Mat my_sum = Mat::zeros(image_in.size(), image_in.type());//最大支持4096*4096分辨率的图像
//Mat my_sqsum= Mat::zeros(image_in.size(), image_in.type());


//Mat my_sum = Mat::zeros(image_in.rows+1,image_in.cols+1, image_in.type());//最大支持4096*4096分辨率的图像
//Mat my_sqsum= Mat::zeros(image_in.rows+1,image_in.cols+1, image_in.type());

Mat my_sum = Mat::zeros(image_in.rows+1,image_in.cols+1, CV_32FC1);//最大支持4096*4096分辨率的图像
Mat my_sqsum= Mat::zeros(image_in.rows+1,image_in.cols+1, CV_32FC1);


//Mat my_sum = Mat_<double>(4096,4096);//最大支持4096*4096分辨率的图像
//Mat my_sqsum = Mat_<long double>(4096,4096);
//Mat my_sqsum= Mat::zeros(image_in.size(), image_in.type());

int get_mean(int h, int w, int neibourhood,int num)
{
    int val;
    switch (num)
    {
        case 1: val = (int)(my_sum.at<float>(h+1,w+1) - my_sum.at<float>(h - neibourhood+1,w+1) - my_sum.at<float>(h+1,w - neibourhood+1) + my_sum.at<float>(h - neibourhood+1,w - neibourhood+1)) / (neibourhood*neibourhood); break;
        case 2: val = (int)(my_sum.at<float>(h+1,w + neibourhood) - my_sum.at<float>(h+1,w) - my_sum.at<float>(h - neibourhood+1,w + neibourhood) + my_sum.at<float>(h - neibourhood+1,w)) / (neibourhood*neibourhood); break;
        case 3: val = (int)(my_sum.at<float>(h + neibourhood,w+1) - my_sum.at<float>(h,w+1) - my_sum.at<float>(h + neibourhood,w - neibourhood+1) + my_sum.at<float>(h,w - neibourhood+1)) / (neibourhood*neibourhood); break;
        case 4: val = (int)(my_sum.at<float>(h + neibourhood,w + neibourhood) - my_sum.at<float>(h + neibourhood,w) - my_sum.at<float>(h,w + neibourhood) + my_sum.at<float>(h,w )) / (neibourhood*neibourhood); break;
    
    }
    return val;
}


int main (int argc, char* argv[]){
    
    image_in.copyTo(image_out);

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
    
    integral(image_in,my_sum,my_sqsum,CV_32F,CV_32F); //计算积分图sumii
//    integral(image_in,my_sum,my_sqsum,CV_64F);
//    cout << "my_sum = " << endl << " " << my_sum << endl << endl;
    
//    cout << "my_sqsum = " << endl << " " << my_sqsum << endl << endl;



    //kuwahara滤波器
    int neibourhood = kernel / 2 + 1;
    float N = neibourhood*neibourhood;
    for (int h = neibourhood; h < height-neibourhood;h++)
    {
        for (int w = neibourhood; w < width-neibourhood;w++)
        {
            //左上
            double L_T_sqsum =  my_sqsum.at<float>(h+1,w+1) - my_sqsum.at<float>(h - neibourhood+1,w+1) - my_sqsum.at<float>(h+1,w - neibourhood+1) + my_sqsum.at<float>(h - neibourhood+1,w - neibourhood+1);
            double L_T_sum = my_sum.at<float>(h+1,w+1) - my_sum.at<float>(h - neibourhood+1,w+1) - my_sum.at<float>(h+1,w - neibourhood+1) + my_sum.at<float>(h - neibourhood+1,w - neibourhood+1);
     
            
            double left_top =( L_T_sqsum - L_T_sum*L_T_sum / N)/N;
//            cout<<"left_top:"<<left_top <<endl;

            //右上
            double R_T_sqsum = my_sqsum.at<float>(h+1,w  + neibourhood) - my_sqsum.at<float>(h+1,w) - my_sqsum.at<float>(h - neibourhood+1,w + neibourhood) + my_sqsum.at<float>(h - neibourhood+1,w);
            double R_T_sum = my_sum.at<float>(h+1,w + neibourhood) - my_sum.at<float>(h+1,w) - my_sum.at<float>(h - neibourhood+1,w + neibourhood) + my_sum.at<float>(h - neibourhood+1,w);
            double right_top =( R_T_sqsum - R_T_sum*R_T_sum / N)/N;
//            cout<<"right_top:"<<right_top <<endl;
            
            //left_bottom
            double L_B_sqsum = my_sqsum.at<float>(h  + neibourhood,w+1) - my_sqsum.at<float>(h,w+1) - my_sqsum.at<float>(h + neibourhood,w - neibourhood+1) + my_sqsum.at<float>(h,w - neibourhood+1);
            double L_B_sum = my_sum.at<float>(h + neibourhood,w+1) - my_sum.at<float>(h,w+1) - my_sum.at<float>(h + neibourhood,w - neibourhood+1) + my_sum.at<float>(h,w - neibourhood+1);
           
            double left_bottom =( L_B_sqsum - L_B_sum*L_B_sum / N)/N;
//            cout<<"left_bottom:"<<left_bottom <<endl;
                                                                                      
            //right_bottom
            double R_B_sqsum = my_sqsum.at<float>(h + neibourhood, w + neibourhood) - my_sqsum.at<float>(h,w  + neibourhood) - my_sqsum.at<float>(h + neibourhood,w ) + my_sqsum.at<float>(h ,w );
            double R_B_sum = my_sum.at<float>(h + neibourhood,w + neibourhood) - my_sum.at<float>(h,w + neibourhood) - my_sum.at<float>(h + neibourhood,w ) + my_sum.at<float>(h ,w );
            
           
            double right_bottom =( R_B_sqsum - R_B_sum*R_B_sum / N)/N;
//            cout<<"right_bottom:"<<right_bottom <<endl;
                   
            
            double min_var = (left_top > right_top) ? right_top : left_top;
            min_var = (left_bottom > min_var) ? min_var : left_bottom;
            min_var = (right_bottom > min_var) ? min_var : right_bottom;
//            cout<<"---";
                                                                                      
                                                                                      
//            uchar kuwahara_val = 0;
            int kuwahara_val = 0;
            if (min_var == left_top)  kuwahara_val = get_mean(h, w,neibourhood, 1);
            else if (min_var == right_top) kuwahara_val=get_mean( h, w, neibourhood, 2);
            else if (min_var == left_bottom) kuwahara_val = get_mean(h, w, neibourhood, 3);
            else if(min_var==right_bottom)  kuwahara_val = get_mean( h, w, neibourhood, 4);
            
            cout <<kuwahara_val<<",";
            
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

    waitKey(0);
    return 1;

    /*}
    else
        return 0;*/


}
#endif

//===================//

#if 0

int main(){

    Mat image=imread("./images/lena.jpg", 0);
        
    
    Mat sumii, sqsumii;
    integral(image,sumii,sqsumii,CV_32F); //计算积分图sumii
    
    
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
  
