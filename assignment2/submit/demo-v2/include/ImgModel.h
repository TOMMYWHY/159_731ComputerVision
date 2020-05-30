//
// Created by Tommy on 2020/5/27.
//

#ifndef BARCODES_IMGMODEL_H
#define BARCODES_IMGMODEL_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv;

struct PointInfo{
    int x;
    int y;
    int b;
    int g;
    int r;
};
class ImgModel {
public:
    Mat Img;
    vector<Point2f> three_circles_points;
    int rightAngleIndex;//todo 没啥用
    float distance;
    Point start_p;
    Point end_p;
    int sub_box_w;
    vector<PointInfo> sub_boxes;
    ImgModel(Mat img);
    void get_3_circles_points( );
    void get_rightAngle_points();
    void reposition_rightAngle();
    void points_swap_to_right_pos();
    void calculate_distance();
    void calculate_points();
    void to_string();
    ~ImgModel();
};


#endif //BARCODES_IMGMODEL_H
