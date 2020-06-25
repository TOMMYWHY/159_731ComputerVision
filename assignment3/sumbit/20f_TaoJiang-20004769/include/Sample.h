//
// Created by Tommy on 2020/6/11.
//

#ifndef GESTURE_SAMPLE_H
#define GESTURE_SAMPLE_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Sample {
public:
    string dirPath;
    string sampleFileName;
    int label;
    vector<double > feature;
    int feature_amount = 20;
    Mat img;

    Sample(Mat test_sample);//full path for single test
    Sample(string dir_path,string sampleFileName);
    ~Sample();
    int get_label();
    void get_feature_rgb();
    void get_feature_hsv();
    void web_cam_get_feature_hsv();
    int iLowH = 0,iLowS=27,iLowV=59, iHighH=179,iHighS=255,iHighV=209;
    void update_ths(int iLowH ,int iLowS,int iLowV, int iHighH, int iHighS,int iHighV);

    vector<double> EllipticFourierDescriptors(vector<Point>& contour);
    void split(const string &str,vector<string> &v,string spacer);

};


#endif //GESTURE_SAMPLE_H
