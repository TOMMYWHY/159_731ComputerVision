//
// Created by Tommy on 2020/6/11.
//

#include "Sample.h"

Sample::Sample(Mat test_sample){
    this->img = test_sample;
    this->get_feature_hsv();
}

Sample::Sample(string dir_path,string sample_file_name) {
    this->dirPath = dir_path;
    this->sampleFileName = sample_file_name;
    this->img = imread( this->dirPath+this->sampleFileName,1);
    init();
}

Sample::~Sample() {
    printf("sample deleted~!");
}

void Sample::init() {
    this->label = get_label();
//    printf("label: %d" , this->label);cout << endl;
//    get_feature_rgb();//for train
//    get_feature_hsv();//for test
}

int Sample::get_label() {
    vector<string> values; //
    this->split(this->sampleFileName,values,"_");
    if(values[0].length()<2){
        return atoi(values[0].c_str());
    }else{
        return atoi(values[1].c_str());
    }
}

void Sample::get_feature_rgb() {
//    Mat img = imread( this->dirPath+this->sampleFileName,1);
//    imshow(this->sampleFileName,img);
//    Mat gray_img,binary_img;
    cvtColor( this->img , this->img , COLOR_BGR2GRAY );
    threshold ( this->img , this->img , 5 , 255 , THRESH_BINARY ) ;
    imshow("binary_img" , this->img ) ;
    vector<vector<Point> > contours ;
    findContours ( this->img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
    Mat drawing = Mat::zeros( this->img.size(), CV_8UC3 );
    Scalar color = CV_RGB( 0, 255,0 );
    int largestcontour=0;
    long int largestsize=0;
    vector< vector<Point> > filterContours;	// 筛选后的轮廓
    vector< Point > hull;	// 凸包络的点集
    for(int i = 0; i< contours.size(); i++ ) {
        if(largestsize < contours[i].size()) {
            largestsize=contours [ i ]. size () ;
            largestcontour=i ;
        }
    }
    drawContours( drawing, contours, largestcontour, color, 1, 8);
    imshow("drawing" , drawing ) ;
//    vector<float> CE;
    vector<float> feature_values = EllipticFourierDescriptors(contours[largestcontour]);
    this->feature =  feature_values;
}
void Sample::get_feature_hsv() {
//    Mat img = imread( this->dirPath+this->sampleFileName,1);
//    medianBlur(this->img, this->img, 5);
//    imshow( "imageROI" , imageROI ) ;
    Mat binary_img,element;
    Mat hsv ( this->img.rows , this->img.cols ,CV_8UC3 ,Scalar::all(0)) ;
    cvtColor(this->img, hsv, COLOR_BGR2HSV);
    inRange(hsv,Scalar(0,66,76),Scalar(179,255,255),binary_img);
    element=getStructuringElement(MORPH_RECT,Size(3,3));
    morphologyEx(binary_img,binary_img,MORPH_OPEN,element);
    morphologyEx(binary_img,binary_img,MORPH_CLOSE,element);

    vector<vector<Point> > contours ;
    findContours ( binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
    Mat drawing = Mat::zeros( binary_img.size(), CV_8UC3 );
    Scalar color = CV_RGB( 0, 255,0 );
    int largestcontour=0;
    long int largestsize=0;
    vector< vector<Point> > filterContours;
    vector< Point > hull;
    for(int i = 0; i< contours.size(); i++ ) {
        if(largestsize < contours[i].size()) {
            largestsize=contours [ i ]. size () ;
            largestcontour=i ;
        }
    }
    drawContours( drawing, contours, largestcontour, color, 1, 8);
//    int m =0;
//    for (int i = 0; i < contours.size(); i++) {
//        if(i==largestcontour){
//            m = contours[largestcontour].size();
//        }
//    }
    imshow("drawing" , drawing ) ;
//    vector<float> feature_values = EllipticFourierDescriptors(contours[m]);
    vector<float> feature_values = EllipticFourierDescriptors(contours[largestcontour]);
    this->feature =  feature_values;
}



vector< float>  Sample::EllipticFourierDescriptors(vector<Point>& contour) {
    vector<float> ax, ay, bx, by;
    vector< float> CE;
    int m = contour.size();

    int n = this->feature_amount;//number of CEs we are interested in computing

    float t=(2*M_PI)/m;
    for (int k = 0; k < n; k++) {
        ax.push_back(0.0);
        ay.push_back(0.0);
        bx.push_back(0.0);
        by.push_back(0.0);
        for (int i = 0; i < m; i++) {
            ax[k] = ax[k] + contour[i].x * cos((k + 1) * t * (i));
            bx[k] = bx[k] + contour[i].x * sin((k + 1) * t * (i));
            ay[k] = ay[k] + contour[i].y * cos((k + 1) * t * (i));
            by[k] = by[k] + contour[i].y * sin((k + 1) * t * (i));
        }
        ax[k] = (ax[k]) / m;
        bx[k] = (bx[k]) / m;
        ay[k] = (ay[k]) / m;
        by[k] = (by[k]) / m;
    }
    for (int k = 0; k < n; k++) {
        CE.push_back(sqrt((ax[k] * ax[k] + ay[k] * ay[k]) / (ax[0] * ax[0] + ay[0] * ay[0])) +
                     sqrt((bx[k] * bx[k] + by[k] * by[k]) / (bx[0] * bx[0] + by[0] * by[0])));
    }
    /*for (int count = 0; count < n && count < CE.size(); count++) {
        printf("%d CE %f ax %f ay %f bx %f by%f \n" ,count, CE[count], ax[count], ay[count], bx[count], by[count] );
    }*/
    return CE;

}
void Sample:: split(const string &str,vector<string> &v,string spacer)
{
    int pos1,pos2;
    int len=spacer.length();
    pos1=0;
    pos2=str.find(spacer);
    while( pos2 != string::npos )
    {
        v.push_back( str.substr(pos1,pos2-pos1) );
        pos1 = pos2 + len;
        pos2 = str.find(spacer,pos1);
    }
    if(pos1 != str.length())
        v.push_back(str.substr(pos1));
}
