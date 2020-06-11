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
    string fullSamplePath;
    string dirPath;
    string sampleFileName;
    int label;
    vector<float > feature;
    int feature_amount = 20;
//    int flag = 0; // 0: trainset; 1:testset;
    Mat img;

    Sample(Mat test_sample);//full path for single test
    Sample(string dir_path,string sampleFileName);
    ~Sample();
    void init();
    int get_label();
    void get_feature_rgb();
    void get_feature_hsv();

    vector<float> EllipticFourierDescriptors(vector<Point>& contour);
    void split(const string &str,vector<string> &v,string spacer);

};


#endif //GESTURE_SAMPLE_H
