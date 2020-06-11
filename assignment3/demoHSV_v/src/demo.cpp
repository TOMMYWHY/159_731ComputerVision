//
// Created by Tommy on 2020/6/5.
//




#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<algorithm>
#include <math.h>

#include<stdlib.h>
#include<stdio.h>
#include <iostream>
#ifdef __APPLE__
#include <sys/uio.h>
#else
#include <sys/io.h>
#endif
#include <unistd.h>
#include <dirent.h>
#include <chrono>
#include <ctime>
#include "LoadData.h"


using namespace std;
using namespace cv;
using namespace cv::ml;
//using namespace ml;
using namespace chrono ;


#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]




void EllipticFourierDescriptors(vector<Point>& contour , vector< float> & CE) ;
vector<float>  EllipticFourierDescriptors(vector<Point>& contour );
//Mat binary_img,element,hsv;
//void test_webcam(Mat imageROI, vector< float> & CE)
Mat test_webcam(Mat imageROI)
{
    Mat binary_img,element;
    medianBlur(imageROI, imageROI, 5);
    imshow( "imageROI" , imageROI ) ;
//    Mat binary_img,element;
    Mat hsv ( imageROI.rows , imageROI.cols ,CV_8UC3 ,Scalar::all(0)) ;
//    imageROI.copyTo(hsv);
    cvtColor(imageROI, hsv, COLOR_BGR2HSV);
//    inRange(hsv,Scalar(0,38,76),Scalar(179,255,255),binary_img);
//    inRange(hsv,Scalar(0,66,76),Scalar(179,255,255),binary_img);
    inRange(hsv,Scalar(0,24,53),Scalar(179,255,163),binary_img);
    element=getStructuringElement(MORPH_RECT,Size(3,3));
    morphologyEx(binary_img,binary_img,MORPH_OPEN,element);
    morphologyEx(binary_img,binary_img,MORPH_CLOSE,element);
    Canny(binary_img,binary_img,20,60);
    imshow("binary_img",binary_img);
    return binary_img;

}

vector<float> find_contours_CES(Mat& binary_img,vector<vector<Point> >& contours){
    findContours ( binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
    Mat drawing = Mat::zeros( binary_img.size(), CV_8UC3 );
    Scalar color = CV_RGB( 0, 255,0 );
    unsigned  int largestcontour=0;
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
    vector<float> CE;
    vector<float> ax, ay, bx, by;
    int n = 30;
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
    return CE;

}


void test_webcam_old(Mat imageROI, vector< float> & CE){
    Mat hsv ( imageROI . rows , imageROI.cols ,CV_8UC3 ,Scalar::all(0)) ;
    cvtColor(imageROI, hsv,COLOR_BGR2HSV);
    imshow( "hsv" , hsv ) ;
    vector<Mat> channels;
    split(hsv, channels);//分割image1的通道
    Mat channelsH = channels[0];//获取通道1
    Mat channelsS = channels[1];//获取通道2
    Mat channelsV = channels[2];
//        blur(channelsS,channelsS, Size(3,3));
//        medianBlur(channelsS,channelsS, Size(3,3));
    medianBlur(channelsS, channelsS, 3);

//        imshow( "channelsH" , channelsH ) ;
    imshow( "channelsS" , channelsS ) ;
//        imshow( "channelsV" , channelsV ) ;

    Mat gray_img, binary_img,obj_img;
    threshold ( channelsS , binary_img , 28 , 255  , THRESH_BINARY ) ;//todo 区域划分不精准
    imshow( "binary_img" , binary_img ) ;

    vector<vector<Point> > contours ;
    findContours ( binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;

//        imshow( "binary_img" , binary_img ) ;
    Mat drawing = Mat::zeros( binary_img.size(), CV_8UC3 );
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
    EllipticFourierDescriptors(contours[largestcontour], CE);
}

static bool read_num_class_data( const string& filename, int var_count, Mat* _data, Mat* _responses );

template<typename T>
static Ptr<T> load_classifier(const string& filename_to_load);
inline TermCriteria TC(int iters, double eps);
static void test_and_save_classifier(const Ptr<StatModel>& model, const Mat& data, const Mat& responses,int ntrain_samples, int rdelta,const string& filename_to_save);
static bool build_mlp_classifier( const string& data_filename,const string& filename_to_save,const string& filename_to_load );


#if 00001
int main (int argc, char *argv[]){
    string data_filename = "./res/all_img.data";

//    string command = "build";
//    string command = "train";
//    string command = "test";
//    string command = "static";
    string command = "webcam";
    if(command == "build"){
 /* step1: Load trainning images and get CE features */

    LoadData all_img("./images/all_img/",data_filename);
    }
    else if(command=="train"){
 /* step2: build model */
        build_mlp_classifier( data_filename, "./res/all_img_ann_model.xml", "" );
    }
    else if(command=="test"){
        ////-load =>test
    build_mlp_classifier( data_filename, "", "./res/all_img_ann_model.xml" );
    }
    else if(command=="static"){
        //// /* step3:single file testing */

    Mat test_img = imread("./images/test/0_A.jpg",1);//0
//        Mat test_img = imread("./images/test/1_A.jpg",1);//0
//        Mat test_img = imread("./images/test/2_A.jpg",1);//2
//        Mat test_img = imread("./images/test/3_A.jpg",1);//2
//        Mat test_img = imread("./images/test/4_A.jpg",1);//3
//        Mat test_img = imread("./images/test/5_A.jpg",1);//5
//        Mat test_img = imread("./images/test/6_A.jpg",1);//5
//        Mat test_img = imread("./images/test/7_A.jpg",1);//2
//        Mat test_img = imread("./images/test/8_A.jpg",1);//5
//        Mat test_img = imread("./images/test/9_A.jpg",1);//5

        LoadData test(test_img);
        vector<double> test_ce = test.test_ce;
        float CE[test_ce.size()] ;
        for (int i = 0; i < test_ce.size(); i++) {
            CE[i] = test_ce[i];
        }
        cout <<"test_ce.size:"<<test_ce.size() <<endl;

        Ptr<ANN_MLP> model;
        model = load_classifier<ANN_MLP>("./res/all_img_ann_model.xml");
        Mat sample1 = (Mat_<float>(1,29) << CE[1],CE[2],CE[3],CE[4],CE[5],CE[6],CE[7],CE[8],CE[9],CE[10],
                CE[11],CE[12],CE[13],CE[14],CE[15],CE[16],CE[17],CE[18],CE[19],
                CE[20],CE[21],CE[22],CE[23],CE[24],CE[25],CE[26],CE[27],CE[28],CE[29]
        );
        cout << "sample1:"<<sample1<< endl;
        float r = model->predict( sample1 );
        cout << "predict result :"<< r <<endl;
        waitKey(0);
    }

    else if(command=="webcam"){
        /*webcam*/
////    /* step4: predict webcam */
        Ptr<ANN_MLP> model;
        model = load_classifier<ANN_MLP>("./res/all_img_ann_model.xml");
        Point area_star = Point(150,100);
        Point area_end =Point(350,350);
        VideoCapture capture;
        capture.open(0);
        capture.set( CAP_PROP_FRAME_WIDTH, 640) ;
        capture.set(CAP_PROP_FRAME_HEIGHT, 480) ;
        Mat frame, imageROI;
        capture >>frame;
        printf( " frame size %d %d nn" , frame.rows , frame.cols) ;
        int key=0;
        double fps =0.0;
//        vector<float> CE;
        while(1){
            system_clock:: time_point start = system_clock::now();
            capture >>frame;
            if(frame.empty()){break;}

            rectangle(frame,area_star,area_end,Scalar(255,102,0),1,8,0);
            imageROI =frame(Rect(area_star.x+1,area_star.y+1,area_end.x-area_star.x-1,area_end.y-area_star.y-1));

            key=waitKey ( 1 ) ;
            if ( key==113 || key==27) return 0 ;
            system_clock:: time_point end = system_clock::now();
            double seconds = std :: chrono :: duration_cast<std :: chrono ::
            microseconds >(end - start ) . count ( ) ;
            fps = 1000000/ seconds;

            Mat binary_img = test_webcam( imageROI);

            vector<vector<Point> > contours ;
//            int largestcontour=0;
            vector<float> CE = find_contours_CES(binary_img,contours);
            if(fps<80.0){

                Mat test_sample = (Mat_<float>(1,29) << CE[1],CE[2],CE[3],CE[4],CE[5],CE[6],CE[7],CE[8],CE[9],CE[10],
                        CE[11],CE[12],CE[13],CE[14],CE[15],CE[16],CE[17],CE[18],CE[19],CE[20],
                        CE[21],CE[22],CE[23],CE[24],CE[25],CE[26],CE[27],CE[28],CE[29]
                );
//                Mat test_sample = (Mat_<float>(1,29) << 2,0.197147,0.145971,0.0661917,0.0288892,0.0317749,0.0269732,0.0174713,0.0131648,0.0103207,0.00973841,0.0167633,0.00933805,0.00782993,0.00773668,0.00552538,0.00906777,0.00613291,0.00633369,0.0059715,0.00516983,0.00230949,0.00398009,0.0040913,0.00119428,0.00201739,0.00173296,0.0011538,0.00132111,0.00119181);
                float r = model->predict( test_sample );
                cout<<"predict result :"<< r<<endl;
                char print_it[100];
                sprintf( print_it, "fps:%2.1f ; Predict: %f" , fps, r) ;
//                sprintf( print_it, "fps:%2.1f ; Predict: %f" , fps, 6.0) ;
                putText ( frame ,print_it, Point( 10 , 30 ) , FONT_HERSHEY_PLAIN,
                          2 , Scalar(255 ,255 ,255) , 2 , 8) ;
//                CE.clear();

//                free(print_it);
            }
            imshow( "WebCam" , frame ) ;
//            waitKey(500);
            waitKey(1);
        }
        waitKey(0);

    }
    return 0;
}


#endif
static bool read_num_class_data( const string& filename, int var_count, Mat* _data, Mat* _responses )
{
    const int M = 1024;
    char buf[M+2];

    Mat el_ptr(1, var_count, CV_32F);
    int i;
    vector<int> responses;

    _data->release();
    _responses->release();

    FILE* f = fopen( filename.c_str(), "rt" );
    if( !f )
    {
        cout << "Could not read the database " << filename << endl;
        return false;
    }

    for(;;)
    {
        char* ptr;
        if( !fgets( buf, M, f ) || !strchr( buf, ',' ) )
            break;
        responses.push_back(buf[0]);
//char test;
//test=buf[0]+65;
//responses.push_back(test);
//        cout << "responses " << buf[0] << " " ;;//<<  endl;
        ptr = buf+2;
        for( i = 0; i < var_count; i++ )
        {
            int n = 0;
            sscanf( ptr, "%f%n", &el_ptr.at<float>(i), &n );
            ptr += n + 1;
        }
//        cout << el_ptr << endl;
        if( i < var_count )
            break;
        _data->push_back(el_ptr);
    }
    fclose(f);
    Mat(responses).copyTo(*_responses);

    cout << "The database " << filename << " is loaded.\n";

    return true;
}

template<typename T>
static Ptr<T> load_classifier(const string& filename_to_load)
{
    // load classifier from the specified file
    Ptr<T> model = StatModel::load<T>( filename_to_load );
    if( model.empty() )
        cout << "Could not read the classifier " << filename_to_load << endl;
    else
        cout << "The classifier " << filename_to_load << " is loaded.\n";

    return model;
}

inline TermCriteria TC(int iters, double eps)
{
    return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
}

static void test_and_save_classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save)
{
    int i, nsamples_all = data.rows;
    double train_hr = 0, test_hr = 0;
    int training_correct_predict=0;
    // compute prediction error on training data
    for( i = 0; i < nsamples_all; i++ )
    {
        Mat sample = data.row(i);
        cout << "Sample: " << responses.at<int>(i)-48 << " row " << data.row(i) << endl;
        float r = model->predict( sample );
        cout << "Predict:  r = " << r << endl;
        if( (int)r == (int)(responses.at<int>(i)-48) ) //prediction is correct
            training_correct_predict++;

        //r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;


        //if( i < ntrain_samples )
        //    train_hr += r;
        //else
        //    test_hr += r;
    }

    //test_hr /= nsamples_all - ntrain_samples;
    //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
    if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);


    if( !filename_to_save.empty() )
    {
        model->save( filename_to_save );
    }
/*************   Example of how to predict a single sample ************************/
/*
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
    Mat sample1 = (Mat_<float>(1,19) <<2,0.318411,0.0986623,0.0787596,0.0859226,0.0376112,0.0230252,0.0266099,0.0438785,0.0190008,0.00619083,0.0229875,0.0169679,0.010826,0.00794063,0.00923449,0.00630885,0.00738152,0.0068503,0.0118594);// 1
    float r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,19) << 2,0.689071,0.365134,0.342835,0.120374,0.0934477,0.118606,0.0988011,0.0784946,0.0355162,0.0492512,0.0494932,0.0173211,0.0304275,0.0190218,0.0213869,0.0124196,0.0146328,0.0151492,0.0074648);//2
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,19) <<2,0.572504,0.400389,0.515727,0.319886,0.111296,0.0968377,0.0429304,0.0310352,0.0835316,0.0244269,0.0558586,0.0385868,0.0230972,0.0167525,0.028727,0.0085215,0.00960673,0.00896232,0.0246249);//3
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
//    sample1 = (Mat_<float>(1,9) << );//4
//    r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,19) <<2,0.618498,0.40394,0.332275,0.197449,0.205624,0.324552,0.194383,0.13853,0.101569,0.0435424,0.0720414,0.0242021,0.0286721,0.041,0.0566495,0.0325877,0.0306484,0.0177473,0.0149883);//5
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
*/

/**********************************************************************/

}

static bool build_mlp_classifier( const string& data_filename,
                                  const string& filename_to_save,
                                  const string& filename_to_load ){
//    const int class_count = 7;//CLASSES //todo
    const int class_count = 10;//CLASSES //todo
    Mat data;
    Mat responses;
//    bool ok = read_num_class_data( data_filename, 9, &data, &responses );//third parameter: FEATURES
//    bool ok = read_num_class_data( data_filename, 19, &data, &responses );//third parameter: FEATURES
    bool ok = read_num_class_data( data_filename, 29, &data, &responses );//third parameter: FEATURES //todo

    if( !ok )
        return ok;

//    cout << "data= " << endl << " " << data << endl << endl;

    Ptr<ANN_MLP> model;
    int nsamples_all = data.rows;
    int ntrain_samples = (int)(nsamples_all*1.0);//SPLIT
    cout << ntrain_samples<<endl;
    // Create or load MLP classifier
    if( !filename_to_load.empty() )
    {
        // load MLP model
        model = load_classifier<ANN_MLP>(filename_to_load);
        if( model.empty() )
            return false;
        //ntrain_samples = 0;
    }
    else{
        //create MLP model

        Mat train_data = data.rowRange(0, ntrain_samples);
        Mat train_responses = Mat::zeros( ntrain_samples, class_count, CV_32F );
//        cout << "train_data= " << endl << " " << train_data << endl << endl;

        // 1. unroll the responses
        cout << "Unrolling the responses...\n";
        for( int i = 0; i < ntrain_samples; i++ )
        {
            int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
//            cout << "labels " << cls_label << endl;
            train_responses.at<float>(i, cls_label) = 1.f;
        }

        // 2. train classifier
        int layer_sz[] = { data.cols, 100,100,100, class_count };
//        int layer_sz[] = { data.cols, 32,64, 128, 256,512,256, 128, 64,32, class_count };//todo
//        int layer_sz[] = { data.cols,64, 128, 256,512,256, 128, 64, class_count };//todo
        cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;//todo sizeof layer_sz 24 sizeof layer_sz[0]) 4
        int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
        cout << " nlayers  " << nlayers << endl;
        Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );

#if 1
        int method = ANN_MLP::BACKPROP;
//        double method_param = 0.001;
//        double method_param = 0.0001;
        double method_param = 0.000001;
        int max_iter = 1000;
//        int max_iter = 500;
//        int max_iter = 300;
//        int max_iter = 100;
//        int max_iter = 50;
#else
        int method = ANN_MLP::RPROP;
        double method_param = 0.1;
        int max_iter = 1000;
#endif

        Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);

        cout << "Training the classifier (may take a few minutes)...\n";
        model = ANN_MLP::create();
        model->setLayerSizes(layer_sizes);
        model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0, 0);//sigmod
        model->setTermCriteria(TC(max_iter,0));
        model->setTrainMethod(method, method_param);
        model->train(tdata);
        cout << endl;
    }
    test_and_save_classifier(model, data, responses, ntrain_samples, 0, filename_to_save);
    return true;
}

//vector<float> ax, ay, bx, by;
//float t;
//int m , n;
void EllipticFourierDescriptors(vector<Point>& contour , vector< float> & CE) {
    vector<float> ax, ay, bx, by;
    int m = contour.size();

     int n = 30;//number of CEs we are interested in computing
//    int n = 20;//number of CEs we are interested in computing
//    int n = 10;//number of CEs we are interested in computing
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
}


vector<float>  EllipticFourierDescriptors(vector<Point>& contour ) {
    vector<float> ax, ay, bx, by;
    int m = contour.size();
    vector< float> CE;
    cout <<"2bbbb"<<endl;
/*    int n = 30;//number of CEs we are interested in computing
//    int n = 20;//number of CEs we are interested in computing
//    int n = 10;//number of CEs we are interested in computing
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
    ax.clear();
    ay.clear();
    bx.clear();
    by.clear();*/
    return CE;
}