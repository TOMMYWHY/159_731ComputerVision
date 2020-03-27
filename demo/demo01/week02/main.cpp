//
// Created by Tommy on 2020-03-07.
//
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
 * blob detector
 *
 * */
#if 1
int main (int argc, char* argv[]){
    /*if(argc!=2){ cout << "needs 1 argument, e.g. image.jpg" << endl ; exit (0) ;}
    Mat imgColor = imread(argv[1],1);
    Mat imgGrey;
    cvtColor(imgColor,imgGrey,COLOR_BGR2GRAY);
    imshow("img1",imgColor);
    imshow("img2",imgGrey);
    imwrite("./results/Exercise1.3_grey.png",imgGrey);*/
    waitKey(0);

    return 0;
}
#endif


/*
 * Exercise 1
 * 3. Modify the program to copy the image to a grey-scale image.
 * Save the grey-scale image with a different format than the original one.
 * */
#if 0
int main (int argc, char* argv[]){
    if(argc!=2){ cout << "needs 1 argument, e.g. image.jpg" << endl ; exit (0) ;}
    Mat imgColor = imread(argv[1],1);
    Mat imgGrey;
    cvtColor(imgColor,imgGrey,COLOR_BGR2GRAY);
    imshow("img1",imgColor);
    imshow("img2",imgGrey);
    imwrite("./results/Exercise1.3_grey.png",imgGrey);
    waitKey(0);

    return 0;
}
#endif
//===================//

/*
 * Exercise 1
 * 2. Write a program to copy an image and mirror it.
 * Show both the original image and the mirrored image simultaneously
 * */
#if 0
int main (int argc, char* argv[]){
    if(argc!=2){ cout << "needs 1 argument, e.g. image.jpg" << endl ; exit (0) ;}
    Mat img1 = imread(argv[1],1);
    Mat img2;
    flip(img1,img2,1);

    imshow("img1",img1);
    imshow("img2",img2);
    waitKey(0);

    return 0;
}
#endif
//===================//

/*
 * Exercise 1
 * 1. Write a program using OpenCV that opens two images and shows them
 * in the screen in sequence (one at a time).
 * */
#if 0
int main (int argc, char* argv[]){
    if(argc!=3){ cout << "needs 2 argument, e.g. image.jpg" << endl ; exit (0) ;}
    Mat img1 = imread(argv[1],1);
    Mat img2 = imread(argv[2],1);
//    namedWindow("img1");
//    namedWindow("img2");
    imshow("img1",img1);
    imshow("img2",img2);
    waitKey(0);

    return 0;
}
#endif
//===================//
