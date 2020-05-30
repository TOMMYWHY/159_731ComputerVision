//
// Created by Tommy on 2020/5/27.
//

#include "ImgModel.h"



ImgModel::~ImgModel() {}

ImgModel::ImgModel(Mat img) {
    this->Img = img;
    this->get_3_circles_points();
    this->get_rightAngle_points();

    if(this->rightAngleIndex != 1){
        this->reposition_rightAngle();
    }
    this->points_swap_to_right_pos();
    this->calculate_distance();
    this->calculate_points();
}



void ImgModel::get_3_circles_points( ) {
    Mat mid_img,gray_img;
    medianBlur(this->Img, mid_img, 5);
    cvtColor(mid_img, gray_img, COLOR_BGR2GRAY);
    vector<Vec3f> circles ;
    vector<Point2f> center_point;
    HoughCircles(gray_img , circles , HOUGH_GRADIENT, 1, 50, 100, 50, 30, 50);
    for( size_t i = 0; i < circles.size(); i++ ) {
        Vec3i c = circles[i];
//        circle( this->Img, Point(c[0], c[1]), c[2], Scalar(255,255,0) , 3, 1);
//        circle(this->Img, Point(c[0], c[1]), 2, Scalar(255,255,0));
        this->three_circles_points.push_back(Point(c[0], c[1]));
    }
}

void ImgModel::get_rightAngle_points() {
    vector<Point2f> points_3 = this->three_circles_points ;
    Vec2f v0(points_3[0].x, -points_3[0].y), v1(points_3[1].x, -points_3[1].y),v2(points_3[2].x,-points_3[2].y);

    Vec2f AA = v0-v1;
    Vec2f BB = v1-v2;
    Vec2f CC = v2-v0;
    Vec2f right_angle;

    float min = abs(AA.dot(BB));
    this->rightAngleIndex =1;
    if(abs( BB.dot(CC)) < min){
        min =abs( BB.dot(CC));
        this->rightAngleIndex =2;
    }
    if(abs( CC.dot(AA))< min){
        min =abs( CC.dot(AA));
        this->rightAngleIndex =0;
    }
}

void ImgModel::reposition_rightAngle() {
    Point2f temp = this->three_circles_points[rightAngleIndex];
    this->three_circles_points.erase(this->three_circles_points.begin()+this->rightAngleIndex);
    this->three_circles_points.insert(this->three_circles_points.begin()+1,temp);
    this->rightAngleIndex = 1;
}

void ImgModel::points_swap_to_right_pos() {
/*to be 0,1,2 */
//    0
//    |\
//    | \
//    |  \
//    1---2
    if(this->three_circles_points[0].x > this->three_circles_points[2].x){
        swap(this->three_circles_points[0],this->three_circles_points[2]);
    }
}

void ImgModel::calculate_distance() {
    float temp_dis[3];
    temp_dis[0] = sqrt(powf((this->three_circles_points[1].x - this->three_circles_points[0].x),2) + powf((this->three_circles_points[1].y - this->three_circles_points[0].y),2));
    temp_dis[1] = sqrt(powf((this->three_circles_points[2].x - this->three_circles_points[1].x),2) + powf((this->three_circles_points[2].y - this->three_circles_points[1].y),2));
    temp_dis[2] = sqrt(powf((this->three_circles_points[0].x - this->three_circles_points[2].x),2) + powf((this->three_circles_points[0].y - this->three_circles_points[2].y),2));
    this->distance = temp_dis[0];
    for (int i = 0; i < 3; i++) {
        if (this->distance > temp_dis[i]) {
            this->distance = temp_dis[i];
        }
    }
}

void ImgModel::calculate_points() {
    this->sub_box_w =int( this->distance /(47-6));
    Point start_p_by_index1,start_p_by_index0;
    start_p_by_index1.x = this->three_circles_points[1].x - 3 * this->sub_box_w;
    start_p_by_index1.y = this->three_circles_points[1].y - this->distance - 3 * this->sub_box_w;

    start_p_by_index0.x = this->three_circles_points[0].x -3*this->sub_box_w;
    start_p_by_index0.y = this->three_circles_points[0].y -3*this->sub_box_w;

    this->start_p.x  = int((start_p_by_index0.x+start_p_by_index1.x)/2); // use 2 point to get mean value to be start point
    this->start_p.y  = int((start_p_by_index0.y+start_p_by_index1.y)/2);

    this->end_p.x = this->start_p.x + this->distance + 6 *this->sub_box_w;
    this->end_p.y = this->start_p.y + this->distance + 6 *this->sub_box_w;
}



void ImgModel::to_string() {
    cout << "mode:"
    << " 3 center points : "<< this->three_circles_points
    << " right angle index : "<< this->rightAngleIndex
    << " right angle distance: "<< this->distance
    << " start_p : "<< this->start_p
    << " end_ p : "<< this->end_p
    << " sub_box_W : "<< this->sub_box_w
    <<endl;
}



