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
using namespace chrono ;


#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]



#if 000
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
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
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
    cout << "Prediction: " << r << endl;

/**********************************************************************/

}

static bool build_mlp_classifier( const string& data_filename,
                      const string& filename_to_save,
                      const string& filename_to_load ){
    const int class_count = 7;//CLASSES //todo
    Mat data;
    Mat responses;
    bool ok = read_num_class_data( data_filename, 9, &data, &responses );//third parameter: FEATURES

    if( !ok ){
        return ok;
    }

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
        int layer_sz[] = { data.cols, 100, 100, class_count };
//        int layer_sz[] = { data.cols, 128, 256, 128, 64, class_count };
        cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;
        int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
        cout << " nlayers  " << nlayers << endl;
        Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );

#if 1
        int method = ANN_MLP::BACKPROP;
        double method_param = 0.001;
//        int max_iter = 300;
        int max_iter = 100;
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


int main (int argc, char *argv[]){


//// /* step1: Load trainning images and get CE features */
//    LoadData test("./images/test/","./res/test.data");
//    LoadData all_img("./images/all_img/","./res/all_img.data");

    cout <<"write done" << endl;

////    /* step2: build model */
/*
 * train a model
 * params: 4 layer 100 100 fully connected
 * method_param = 0.001, epoch=100
 * */
   /* string data_filename = "./res/all_img.data";
    string filename_to_save = "./res/all_img_ann_model.xml";
    string filename_to_load = "";
    build_mlp_classifier( data_filename, filename_to_save, filename_to_load );*/


////    /* step3: predict test img */
    Mat test_img = imread("./images/test/9_B.jpg",1);
    imshow("test_img",test_img);
    LoadData test;
    vector<double> test_ce = test.single_img_for_test(test_img);
    float CE[test_ce.size()] ;
    for (int i = 0; i < test_ce.size(); i++) {
        CE[i] = test_ce[i];
    }

    Ptr<ANN_MLP> model;
    model = load_classifier<ANN_MLP>("./res/all_img_ann_model.xml");
    Mat test_sample = (Mat_<float>(1,9) << CE[1],CE[2],CE[3],CE[4],CE[5],CE[6],CE[7],CE[8],CE[9]);
    cout << "sample1:"<<test_sample<< endl;
    Mat response_mat;
    float r = model->predict( test_sample );
    cout << "predict result :"<< r <<endl;


//    waitKey(0);

    return 0;
}
#endif

// binary write
#if 00
int main(){
    int a[5] = {1,2,3,4,5};
    int b[5];

    ofstream ouF;
    ouF.open("./res/me.data", std::ofstream::binary);
    ouF.write(reinterpret_cast<const char*>(a), sizeof(int)*5);
    ouF.close();

    cout <<"write func test" << endl;
    return 0;
}
#endif

//text write
#if 00
int main(){
    int a[5] = {1,2,3,4,5};
    int b[5];


    ofstream file;

    file.open("./res/me2.data",ios::app);
    for(int i=0;i< 5;i++){
        file<<a[i] << ",";
    }
//    file<< "\n";
    file<<endl;
    file.close();

    cout <<"write func test" << endl;
    return 0;
}
#endif

//get file name
#if 00
/*void getFiles(string path, vector<string>& files)
{
    //文件句柄
    long hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;

    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                    getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
            }
        }while(_findnext(hFile, &fileinfo)  == 0);
        _findclose(hFile);
    }
}*/

vector<string> getFiles(string cate_dir){
    vector<string> files;//存放文件名

    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir=opendir(cate_dir.c_str())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8)    ///file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            files.push_back(ptr->d_name);
        else if(ptr->d_type == 10)    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            continue;
        else if(ptr->d_type == 4)    ///dir
        {
            files.push_back(ptr->d_name);
            /*
                memset(base,'\0',sizeof(base));
                strcpy(base,basePath);
                strcat(base,"/");
                strcat(base,ptr->d_nSame);
                readFileList(base);
            */
        }
    }
    closedir(dir);
    sort(files.begin(), files.end());
    return files;

}

int main(){
    vector<string> files;

    char * filePath = "./images/test";
    vector<string> files_name =  getFiles( filePath );

    // print the files get
    for(int j=0; j< files_name.size(); ++j)
    {
        cout << files_name[j] << endl;
    }
    cout <<"get file name" << endl;
    return 0;
}
#endif

//webcam
#if 000

int main(){
    Point area_star = Point(150,100);
    Point area_end =Point(350,350);

    VideoCapture capture;
    capture.open(0);
    namedWindow("WebCam",1);
    capture.set( CAP_PROP_FRAME_WIDTH, 640) ;
    capture.set(CAP_PROP_FRAME_HEIGHT, 480) ;
    Mat frame;
    capture >>frame;
    printf( " frame size %d %d nn" , frame.rows , frame.cols) ;
    int key=0;
    double fps =0.0;
    while(1){
        system_clock:: time_point start = system_clock::now();
        capture >>frame;
        if(frame.empty()){
            break;
        }
        char print_it[100];
        sprintf( print_it, "%2.1f " , fps ) ;
        putText ( frame ,print_it, Point( 10 , 30 ) , FONT_HERSHEY_PLAIN,
        2 , Scalar(255 ,255 ,255) , 2 , 8) ;


        rectangle(frame,area_star,area_end,Scalar(255,102,0),3,8,0);

        imshow( "WebCam" , frame ) ;
        key=waitKey ( 1 ) ;
        if ( key==113 || key==27) return 0 ;
        system_clock:: time_point end = system_clock::now();
        double seconds = std :: chrono :: duration_cast<std :: chrono ::
        microseconds >(end - start ) . count ( ) ;
        fps = 1000000/ seconds;
//        cout << " frames: " << fps << "; seconds:" << seconds << endl ;
        if(fps<80.0){
            cout << " fps <80, get area:" << seconds << endl ;

        }
    }
    return 0;
}
#endif

//截取area
#if 0001
int main(){
    Mat img = imread("./images/test/0_A.jpg",1);
    Point area_star = Point(150,100);
    Point area_end =Point(350,350);
    Mat imageROI=img(Rect(area_star.x,area_star.y,area_end.x-area_star.x,area_end.y-area_star.y));


    rectangle(img,area_star,area_end,Scalar(255,102,0),3,8,0);
    imshow("img",img);
    imshow("imageROI",imageROI);
    waitKey(0);
}
#endif