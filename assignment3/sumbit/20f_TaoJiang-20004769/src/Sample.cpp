//
// Created by Tommy on 2020/6/11.
//

#include "Sample.h"

/*test single image*/
Sample::Sample(Mat test_sample){
    this->img = test_sample;
}

/* whole file images*/
Sample::Sample(string dir_path,string sample_file_name) {
    this->dirPath = dir_path;
    this->sampleFileName = sample_file_name;
    this->img = imread( this->dirPath+this->sampleFileName,1);
//    init();
    this->label = get_label();
}

Sample::~Sample() {
    printf("sample deleted~!");
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
    cvtColor( this->img , this->img , COLOR_BGR2GRAY );
    threshold ( this->img , this->img , 5 , 255 , THRESH_BINARY ) ;
    imshow("binary_img" , this->img ) ;
    vector<vector<Point> > contours ;
    findContours ( this->img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
    Mat drawing = Mat::zeros( this->img.size(), CV_8UC3 );
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
    imshow("drawing" , drawing ) ;
    vector<double> feature_values = EllipticFourierDescriptors(contours[largestcontour]);
    this->feature =  feature_values;
}
void Sample::get_feature_hsv() {
    Mat binary_img,element;
    Mat hsv ( this->img.rows , this->img.cols ,CV_8UC3 ,Scalar::all(0)) ;
    cvtColor(this->img, hsv, COLOR_BGR2HSV);
    imshow("hsv",hsv);
    //todo

    inRange(hsv,Scalar(0,63,100),Scalar(179,255,255),binary_img);
    imshow("binary_img",binary_img);
//    int iLowH = 0,iLowS=63,iLowV=100, iHighH=179,iHighS=255,iHighV=255;

//    int iLowH = 0,iLowS=66,iLowV=82, iHighH=179,iHighS=255,iHighV=255;


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
    imshow("drawing" , drawing ) ;
    vector<double> feature_values = EllipticFourierDescriptors(contours[largestcontour]);
    this->feature =  feature_values;
}

void Sample::update_ths(int iLowH ,int iLowS,int iLowV, int iHighH, int iHighS,int iHighV)
{
    this->iLowH = iLowH,this->iLowS=iLowS,this->iLowV=iLowV, this->iHighH=iHighH,this->iHighS=iHighS,this->iHighV=iHighV;
}

void Sample::web_cam_get_feature_hsv(){
    Mat binary_img,element;
    medianBlur(this->img, this->img, 5);
    imshow( "imageROI" , this->img ) ;
//    Mat binary_img,element;
    Mat hsv ( this->img.rows , this->img.cols ,CV_8UC3 ,Scalar::all(0)) ;
    cvtColor(this->img, hsv, COLOR_BGR2HSV);
    imshow( "HSV" , hsv) ;
    inRange(hsv,Scalar(iLowH,iLowS,iLowV),Scalar(iHighH,iHighS,iHighV),binary_img);
    element=getStructuringElement(MORPH_RECT,Size(3,3));
    morphologyEx(binary_img,binary_img,MORPH_OPEN,element);
    morphologyEx(binary_img,binary_img,MORPH_CLOSE,element);
    Canny(binary_img,binary_img,20,60);
    imshow("binary_img",binary_img);
     vector<vector<Point> > contours ;

    findContours ( binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
    Mat drawing = Mat::zeros( binary_img.size(), CV_8UC3 );
    Scalar color = CV_RGB( 0, 255,0 );
    unsigned  int largestcontour=0;
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
    imshow("drawing" , drawing ) ;
    vector<double> CE;
    vector<double> ax, ay, bx, by;
    int n = this->feature_amount;
    int m = 0;
    for (int i = 0; i < contours.size(); i++) {
        if(i==largestcontour){
            m = contours[largestcontour].size();
        }
    }
    float t=(2*M_PI)/m;
    for (int k = 0; k < n; k++) {
        ax.push_back(0.0);
        ay.push_back(0.0);
        bx.push_back(0.0);
        by.push_back(0.0);
        for (int i = 0; i < m; i++) {
            ax[k] = ax[k] + contours[largestcontour][i].x * cos((k + 1) * t * (i));
            bx[k] = bx[k] + contours[largestcontour][i].x * sin((k + 1) * t * (i));
            ay[k] = ay[k] + contours[largestcontour][i].y * cos((k + 1) * t * (i));
            by[k] = by[k] + contours[largestcontour][i].y * sin((k + 1) * t * (i));
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

    this->feature=CE;
};



vector< double>  Sample::EllipticFourierDescriptors(vector<Point>& contour) {
    vector<double> ax, ay, bx, by;
    vector< double> CE;
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
