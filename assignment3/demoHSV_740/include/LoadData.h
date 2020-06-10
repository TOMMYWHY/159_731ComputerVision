//
// Created by Tommy on 2020/6/7.
//

#ifndef GESTURE_LOADDATA_H
#define GESTURE_LOADDATA_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"

using namespace cv;

using namespace std;
#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]

template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}
struct Feature{
    int index;
    double value;
};
struct Sample{
    string label;
    vector<vector<double> >features;
    vector<double> feature;
    string file_name;
    string path;
};

class LoadData {
public:
    string filePath;
    string dataFrame_path;
    vector<Sample> samples;


//    LoadData(char * test_img_path);
    LoadData(Mat imageROI);

    LoadData( string filePath, string dataFrame_path);
    ~LoadData();
    void get_files_name();
//    void get_feature(Sample sample);
    vector<double> get_single_feature_HSV(Mat imageROI);
    vector<double> get_single_feature_RGB(Mat imageROI);

    void get_features();//v2
    void get_features_RGB();//v1
//    Mat get_obj_img(Mat org_img);
    vector<double>  get_single_obj_feature(Mat & obj_img);
    vector<double> EllipticFourierDescriptors(vector<Point>& contour , vector< float> & CE);
    void save_feature();

//    vector<double> single_img_for_test(Mat & test_img);
    vector<double>  test_ce;

private:
    vector<string> getFiles(string cate_dir);
    void split(const string &str,vector<string> &v,string spacer);

};


#endif //GESTURE_LOADDATA_H
