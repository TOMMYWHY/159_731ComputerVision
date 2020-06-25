/*
 *  Assignment 3:Recognising gestures from images, 159.731, S1 2020
 *  Family Name: JIANG , Given Name: TAO , Student ID:20004769,
 *
 *  run ./gesture train                  : extract trainset images features save to file and build a ANN model
 *  run ./gesture test                   : extract testset images features save to file and test the ANN model
 *  run ./gesture                        : use web-camera to  predict
 *  run ./gesture ./images/test/0_A.jpg  : processes a single static image
 *
 *
 *  the model save as ./res/gesture.xml
 *  the trainset features save as ./res/gesture_train.data
 *  the testset features save as ./res/gesture_test.data
 *
 *
 * ANN model:
 * 2 hidden fully connected layers with 100 Neurons
 * epochs:500
 * learning rate:0.0001
 * */




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
#include "Sample.h"
//#include "utils.h"

using namespace std;
using namespace cv;
using namespace cv::ml;
using namespace chrono ;


// This function reads data and responses from the file <filename>
static bool read_num_class_data( const string& filename, int var_count, Mat* _data, Mat* _responses );
template<typename T>
static Ptr<T> load_classifier(const string& filename_to_load);
inline TermCriteria TC(int iters, double eps);
static void test_and_save_classifier(const Ptr<StatModel>& model, const Mat& data, const Mat& responses,int ntrain_samples, int rdelta,const string& filename_to_save);
static bool build_mlp_classifier( const string& data_filename,const string& filename_to_save,const string& filename_to_load );

vector<string> getFiles(const string& train_sample_dir_path);
void extract_features_save_to_file(const string& train_sample_dir_path,const string& data_filename,vector<string> files,int flag);

void training(string data_filename,string sample_dir_path,string model_file);
void testing(string data_filename,string sample_dir_path,string model_file);
void single_test(Mat test_img,string model_file);

/*track bar control*/
int iLowH = 0,iLowS=27,iLowV=82, iHighH=179,iHighS=255,iHighV=255;
//iLowH:0 iLowS: 27 iLowV: 82 iHighH: 179 iHighS: 255 iHighV: 255

void callback(int,void*){
    iLowH=getTrackbarPos("Low H","color");
    iHighH=getTrackbarPos("High H","color");
    iLowS=getTrackbarPos("Low S","color");
    iHighS=getTrackbarPos("High S","color");
    iLowV=getTrackbarPos("Low V","color");
    iHighV=getTrackbarPos("High V","color");
}
void trackbar_control(){
    namedWindow("color");
    createTrackbar("Low H","color",&iLowH,179,callback);//Hue0-179
    createTrackbar("High H","color",&iHighH,179,callback);
    createTrackbar("Low S","color",&iLowS,255,callback);
    createTrackbar("High S","color",&iHighS,255,callback);
    createTrackbar("Low V","color",&iLowV,255,callback);
    createTrackbar("High V","color",&iHighV,255,callback);
    callback(0,0);
}

int main(int argc, char *argv[]){
    string data_filename, sample_dir_path;
    string model_file = "./res/gesture.xml";
    string command = "";
    string single_file_path="";

    if(argc == 1){
        command = "webcam";
    }
    else if (argc == 2){
        if( strcmp(argv[1],"train") == 0 ){
            command = "train";
        }
        else if( strcmp(argv[1],"test") == 0 ){
            command = "test";
        }
        else{
            command = "static";
            single_file_path = argv[1];
        }
    }


//     command = "train";
//     command = "test";
//     command = "static";
//     command = "webcam";
//        command="testing";

     if(command=="train"){
        /* step1: build model and save modeling file */
         cout <<"train model." <<endl;
         training( data_filename, sample_dir_path, model_file);
    }
    else if(command=="test"){
         /* step2: predict test file iamges */
         cout <<"test model." <<endl;
         testing( data_filename, sample_dir_path, model_file);
    }
    else if(command=="static"){
         /* step3: predict one iamge */
         cout <<"static model." <<endl;
//         single_file_path ="./images/test/4_C.jpg";
//         single_file_path ="./images/test/9_D.jpg";
//         single_file_path ="./images/test/4_D.jpg";
         Mat test_img = imread(single_file_path,1);//0
         single_test( test_img, model_file);
     }

    else if(command=="webcam"){
        cout <<"web-camera model." <<endl;
         /* step4: webcam predict */
         Ptr<ANN_MLP> model;
         model = load_classifier<ANN_MLP>(model_file);
         Point area_star = Point(150,100);
         Point area_end =Point(380,380);
         VideoCapture capture;
         capture.open(0);
         capture.set( CAP_PROP_FRAME_WIDTH, 640) ;
         capture.set(CAP_PROP_FRAME_HEIGHT, 480) ;
         Mat frame, imageROI;
         capture >>frame;
         printf( " frame size %d %d nn" , frame.rows , frame.cols) ;
         int key=0;
         double fps =0.0;
         while(1){
             system_clock:: time_point start = system_clock::now();
             capture >>frame;
             if(frame.empty()){break;}
             rectangle(frame,area_star,area_end,Scalar(255,0,0),1,8,0);
             imageROI =frame(Rect(area_star.x+1,area_star.y+1,area_end.x-area_star.x-1,area_end.y-area_star.y-1));

             trackbar_control();
             Sample* p_sample = new Sample(imageROI);
             p_sample->update_ths(iLowH ,iLowS,iLowV, iHighH,iHighS,iHighV);
             p_sample->web_cam_get_feature_hsv();
             vector<double> CE = p_sample->feature;
             cout <<"iLowH:"<<iLowH<<"iLowS:"<<iLowS<<"iLowV:"<<iLowV <<"iHighH:"<<iHighH <<"iHighS:"<<iHighS <<"iHighV:"<<iHighV<<endl;

             key=waitKey ( 1 ) ;
             if ( key==113 || key==27) return 0 ;
             system_clock:: time_point end = system_clock::now();
             double seconds = std :: chrono :: duration_cast<std :: chrono ::
             microseconds >(end - start ) . count ( ) ;
             fps = 1000000/ seconds;
             if(fps<80.0){
                 /*Mat test_sample = (Mat_<double>(1,19) << CE[1],CE[2],CE[3],CE[4],CE[5],CE[6],CE[7],CE[8],CE[9],CE[10],
                         CE[11],CE[12],CE[13],CE[14],CE[15],CE[16],CE[17],CE[18],CE[19]
                 );*/
                 Mat test_sample = (Mat_<double>(1,29) <<
                         CE[1],CE[2],CE[3],CE[4],CE[5],CE[6],CE[7],CE[8],CE[9],CE[10],
                         CE[11],CE[12],CE[13],CE[14],CE[15],CE[16],CE[17],CE[18],CE[19],CE[20],
                         CE[21],CE[22],CE[23],CE[24],CE[25],CE[26],CE[27],CE[28],CE[29]
                 );
                 float r = model->predict( test_sample );
                 cout<<"predict result :"<< r<<endl;
                 char print_it[100];
                 sprintf( print_it, "fps:%2.1f ; Predict: %f" , fps, r) ;
//                sprintf( print_it, "fps:%2.1f ; Predict: %f" , fps, 6.0) ;
                 putText ( frame ,print_it, Point( 10 , 30 ) , FONT_HERSHEY_PLAIN,
                           2 , Scalar(255 ,255 ,255) , 2 , 8) ;
             }
             imshow( "WebCam" , frame ) ;
         }
         waitKey(1);
     }

    waitKey(0);
    return 0;
}


void training(string data_filename,string sample_dir_path,string model_file){
    int flag = 0; //0: trainset;
    data_filename = "./res/gesture_train.data";
    sample_dir_path = "./images/all_img/";
    cout << "extracting train features....."<<endl;
    vector<string> files =  getFiles(sample_dir_path);
    extract_features_save_to_file( sample_dir_path, data_filename, files, flag); //get rgb features
    cout << "dataframe saved at : "<<data_filename  <<endl;
    build_mlp_classifier( data_filename, model_file, "" );
}
void testing(string data_filename,string sample_dir_path,string model_file){
    data_filename = "./res/gesture_test.data";
    sample_dir_path = "./images/test/";
    int flag = 1; //1: testset; //
    cout << "extracting test features....."<<endl;
    vector<string> files =  getFiles(sample_dir_path);
    extract_features_save_to_file( sample_dir_path, data_filename, files,flag);
    cout << "dataframe saved at : "<<data_filename  <<endl;
    build_mlp_classifier( data_filename, "", model_file );
}
void single_test(Mat test_img,string model_file){
    Sample* p_sample = new Sample(test_img);
    p_sample->get_feature_hsv();
    vector<double> CE = p_sample->feature;
    for (int i = 0; i < CE.size(); i++) {
        cout <<CE[i]<<",";
    }
    cout<<endl;
   /* Mat test_sample = (Mat_<double>(1,p_sample->feature_amount-1) << CE[1],CE[2],CE[3],CE[4],CE[5],CE[6],CE[7],CE[8],CE[9],CE[10],
            CE[11],CE[12],CE[13],CE[14],CE[15],CE[16],CE[17],CE[18],CE[19]
    );*/

    Mat test_sample = (Mat_<double>(1,p_sample->feature_amount-1) <<
            CE[1],CE[2],CE[3],CE[4],CE[5],CE[6],CE[7],CE[8],CE[9],CE[10],
            CE[11],CE[12],CE[13],CE[14],CE[15],CE[16],CE[17],CE[18],CE[19],CE[20],
            CE[21],CE[22],CE[23],CE[24],CE[25],CE[26],CE[27],CE[28],CE[29]
    );

//    Mat test_sample = (Mat_<float>(1,p_sample->feature_amount-1) << 2, 0.94069302, 0.183801, 0.381843, 0.36048001, 0.123997, 0.051369902, 0.0596321, 0.058735199, 0.038410701, 0.024405001, 0.033344898, 0.026175899, 0.0326165, 0.018029099, 0.0136418, 0.026245801, 0.0121548, 0.0137993
//    );
    Ptr<ANN_MLP> model;
    model = load_classifier<ANN_MLP>(model_file);
    cout << "single sample test:"<<test_sample<< endl;
    float r = model->predict( test_sample );
    cout << "predict result :"<< r <<endl;
    char print_it[100];
    sprintf( print_it, " Predict: %d" , int(r)) ;
    putText ( test_img ,print_it, Point( 10 , 30 ) , FONT_HERSHEY_PLAIN,
              2 , Scalar(255 ,0 ,0) , 2 , 8);
    imshow("test_img",test_img);
}

/*=================utils function=======================*/

vector<string> getFiles(const string& sample_dir_path){
    vector<string> files;
    DIR *dir;
    struct dirent *ptr;
    char base[1000];
    if ((dir=opendir(sample_dir_path.c_str())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }
    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
            continue;
        else if(ptr->d_type == 8){
            ///file
//            printf("d_name:%s/%s\n",base,ptr->d_name);
            files.push_back(ptr->d_name);
        }
        else if(ptr->d_type == 10)    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            continue;
        else if(ptr->d_type == 4)    ///dir
        {
            files.push_back(ptr->d_name);

        }
    }
    closedir(dir);

    sort(files.begin(), files.end());
    return files;

}
void extract_features_save_to_file(const string& sample_dir_path,const string& data_filename,vector<string> files,int flag){
    ofstream w_file;
    w_file.open(data_filename);
    for (int i = 0; i < files.size(); i++) {
//        cout << train_sample_dir_path << files[i] <<endl;
        Sample* p_sample = new Sample(sample_dir_path,files[i]);

        cout<< p_sample->label<<"," ;
        w_file << p_sample->label <<",";
        if(flag == 0){
            p_sample->get_feature_rgb();
        } else if (flag==1){
            p_sample->get_feature_hsv();
        }
        for (int j = 0; j < p_sample->feature.size(); j++) {
            cout<< p_sample->feature[j] <<"," ;
            w_file << p_sample->feature[j]  <<",";
        }
        cout <<endl;
        w_file<<endl;
    }
    w_file.close();
    cout << endl;
}

/*========================================*/
// This function reads data and responses from the file <filename>
static bool
read_num_class_data( const string& filename, int var_count,
                     Mat* _data, Mat* _responses )
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
//cout << "responses " << buf[0] << " " ;;//<<  endl;
//        cout<<buf[0]<<endl;
        cout << "responses=> label: " << buf[0] << " " ;;//<<  endl;
        ptr = buf+2;
        for( i = 0; i < var_count; i++ )
        {
            int n = 0;
            sscanf( ptr, "%f%n", &el_ptr.at<float>(i), &n );
            ptr += n + 1;
        }
//cout << el_ptr << endl;
        cout <<"features size:"<< el_ptr.size() << "; features value:" << el_ptr << endl;
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
//        cout << "Sample org: " << sample << endl;

        cout << "Sample-> label:" << responses.at<int>(i)-48 << " row: " << data.row(i) << endl;
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
//    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
    printf("number of train_samples: %d ; training_correct_predict: %d \n",ntrain_samples, training_correct_predict);
    if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);


    if( !filename_to_save.empty() )
    {
        cout << "X_train predict accuracy result: "<<training_correct_predict*100.0/ntrain_samples <<endl;
        model->save( filename_to_save );
        cout << "model saved at: "<<filename_to_save <<endl;

    }
/*************   Example of how to predict a single sample ************************/
/*// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
    Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
    float r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
//    sample1 = (Mat_<float>(1,9) << );//4
//    r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;
    sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
    r = model->predict( sample1 );
    cout << "Prediction: " << r << endl;*/

/**********************************************************************/

}

static bool
build_mlp_classifier( const string& data_filename,
                      const string& filename_to_save,
                      const string& filename_to_load )
{
    const int class_count = 10;//CLASSES
    Mat data;
    Mat responses;

//    bool ok = read_num_class_data( data_filename, 19, &data, &responses );//third parameter: FEATURES
    bool ok = read_num_class_data( data_filename, 29, &data, &responses );//third parameter: FEATURES
    if( !ok )
        return ok;

    Ptr<ANN_MLP> model;

    int nsamples_all = data.rows;
    int ntrain_samples = (int)(nsamples_all*1.0);//SPLIT

    // Create or load MLP classifier
    if( !filename_to_load.empty() )
    {
        model = load_classifier<ANN_MLP>(filename_to_load);
        if( model.empty() )
            return false;
        //ntrain_samples = 0;
    }
    else
    {
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //
        // MLP does not support categorical variables by explicitly.
        // So, instead of the output class label, we will use
        // a binary vector of <class_count> components for training and,
        // therefore, MLP will give us a vector of "probabilities" at the
        // prediction stage
        //
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        Mat train_data = data.rowRange(0, ntrain_samples);
        cout <<"train_data:"<<train_data<<endl;
        Mat train_responses = Mat::zeros( ntrain_samples, class_count, CV_32F );

        // 1. unroll the responses
        cout << "Unrolling the responses...\n";
        for( int i = 0; i < ntrain_samples; i++ )
        {
            int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
            cout << "labels: " << cls_label << endl; // all labels
            train_responses.at<float>(i, cls_label) = 1.f;
        }
        cout <<"train_responses:"<<train_responses<<endl;


        // 2. train classifier
//        int layer_sz[] = { data.cols, 100, 100 ,100, class_count }; // 97.7143
//        int layer_sz[] = { data.cols, 100, 100,100 ,100, class_count }; //95.4286
        int layer_sz[] = { data.cols, 100, 100, class_count }; //98.7143
//        int layer_sz[] = { data.cols, 100, class_count }; //98.4286 todo ?????
//        int layer_sz[] = { data.cols,32, 64, 128,256,128 ,64,32, class_count }; //
//        int layer_sz[] = { data.cols, 128,256,256,128, class_count }; // 92.8571
//        int layer_sz[] = { data.cols, 256,512,512,256, class_count }; //94
        cout <<" data.cols:"<< data.cols<<"; class_count:"<<class_count<< endl;
        cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;
        int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0])); // 4 layers
        cout << " nlayers  " << nlayers << endl;
        Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );
        cout <<"layer_sizes:"<<layer_sizes<<endl;

#if 1
        int method = ANN_MLP::BACKPROP;
//        double method_param = 0.001;
        double method_param = 0.0001;
        int max_iter = 500;
#else
        int method = ANN_MLP::RPROP;
        double method_param = 0.1;
        int max_iter = 1000;
#endif

        Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);//X_train y_train

        cout << "Training the classifier (may take a few minutes)...\n";
        model = ANN_MLP::create();
        model->setLayerSizes(layer_sizes);
        model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0, 0);
        model->setTermCriteria(TC(max_iter,0));
        model->setTrainMethod(method, method_param);
        model->train(tdata);
        cout << "==========================================================="<<endl;
        cout << endl;
    }

    //test_and_save_classifier(model, data, responses, ntrain_samples, 'A', filename_to_save);
    test_and_save_classifier(model, data, responses, ntrain_samples, 0, filename_to_save);//model,X_train,y_train,numberOfTrain
    return true;
}


