////
//// Created by Tommy on 2020/6/11.
////
//#include "utils.h"
//#include "opencv2/core/core.hpp"
//#include "opencv2/ml/ml.hpp"
//
//#include <cstdio>
//#include <vector>
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//using namespace cv::ml;
//// This function reads data and responses from the file <filename>
//static bool
//read_num_class_data( const string& filename, int var_count,
//                     Mat* _data, Mat* _responses )
//{
//    const int M = 1024;
//    char buf[M+2];
//
//    Mat el_ptr(1, var_count, CV_32F);
//    int i;
//    vector<int> responses;
//
//    _data->release();
//    _responses->release();
//
//    FILE* f = fopen( filename.c_str(), "rt" );
//    if( !f )
//    {
//        cout << "Could not read the database " << filename << endl;
//        return false;
//    }
//
//    for(;;)
//    {
//        char* ptr;
//        if( !fgets( buf, M, f ) || !strchr( buf, ',' ) )
//            break;
//        responses.push_back(buf[0]);
////char test;
////test=buf[0]+65;
////responses.push_back(test);
////cout << "responses " << buf[0] << " " ;;//<<  endl;
//        cout << "responses=> label: " << buf[0] << " " ;;//<<  endl;
//        ptr = buf+2;
//        for( i = 0; i < var_count; i++ )
//        {
//            int n = 0;
//            sscanf( ptr, "%f%n", &el_ptr.at<float>(i), &n );
//            ptr += n + 1;
//        }
////cout << el_ptr << endl;
//        cout <<"features size:"<< el_ptr.size() << "; features value:" << el_ptr << endl;
//        if( i < var_count )
//            break;
//        _data->push_back(el_ptr);
//    }
//    fclose(f);
//    Mat(responses).copyTo(*_responses);
//
//    cout << "The database " << filename << " is loaded.\n";
//
//    return true;
//}
//
//template<typename T>
//static Ptr<T> load_classifier(const string& filename_to_load)
//{
//    // load classifier from the specified file
//    Ptr<T> model = StatModel::load<T>( filename_to_load );
//    if( model.empty() )
//        cout << "Could not read the classifier " << filename_to_load << endl;
//    else
//        cout << "The classifier " << filename_to_load << " is loaded.\n";
//
//    return model;
//}
//
//inline TermCriteria TC(int iters, double eps)
//{
//    return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
//}
//
//static void test_and_save_classifier(const Ptr<StatModel>& model,
//                                     const Mat& data, const Mat& responses,
//                                     int ntrain_samples, int rdelta,
//                                     const string& filename_to_save)
//{
//    int i, nsamples_all = data.rows;
//    double train_hr = 0, test_hr = 0;
//    int training_correct_predict=0;
//    // compute prediction error on training data
//    for( i = 0; i < nsamples_all; i++ )
//    {
//        Mat sample = data.row(i);
////        cout << "Sample org: " << sample << endl;
//
//        cout << "Sample-> label:" << responses.at<int>(i)-48 << " row: " << data.row(i) << endl;
//        float r = model->predict( sample );
//        cout << "Predict:  r = " << r << endl;
//        if( (int)r == (int)(responses.at<int>(i)-48) ) //prediction is correct
//            training_correct_predict++;
//
//        //r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;
//
//
//        //if( i < ntrain_samples )
//        //    train_hr += r;
//        //else
//        //    test_hr += r;
//    }
//
//    //test_hr /= nsamples_all - ntrain_samples;
//    //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
////    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
//    printf("number of train_samples: %d ; training_correct_predict: %d \n",ntrain_samples, training_correct_predict);
//    if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);
//
//
//    if( !filename_to_save.empty() )
//    {
//        cout << "X_train predict accuracy result: "<<training_correct_predict*100.0/ntrain_samples <<endl;
//        model->save( filename_to_save );
//        cout << "model saved at: "<<filename_to_save <<endl;
//
//    }
///*************   Example of how to predict a single sample ************************/
///*// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
//    //Mat sample = data.row(i);
//    Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
//    float r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;
//    sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
//    r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;
//    sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
//    r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;
////    sample1 = (Mat_<float>(1,9) << );//4
////    r = model->predict( sample1 );
////    cout << "Prediction: " << r << endl;
//    sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
//    r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;
//    sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
//    r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;
//    sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
//    r = model->predict( sample1 );
//    cout << "Prediction: " << r << endl;*/
//
///**********************************************************************/
//
//}
//
//
//static bool build_mlp_classifier( const string& data_filename,
//                                  const string& filename_to_save,
//                                  const string& filename_to_load ){
////    const int class_count = 7;//CLASSES //todo
//    const int class_count = 10;//CLASSES //todo
//    Mat data;
//    Mat responses;
////    bool ok = read_num_class_data( data_filename, 9, &data, &responses );//third parameter: FEATURES
////    bool ok = read_num_class_data( data_filename, 19, &data, &responses );//third parameter: FEATURES
//    bool ok = read_num_class_data( data_filename, 29, &data, &responses );//third parameter: FEATURES //todo
//
//    if( !ok )
//        return ok;
//
////    cout << "data= " << endl << " " << data << endl << endl;
//
//    Ptr<ANN_MLP> model;
//    int nsamples_all = data.rows;
//    int ntrain_samples = (int)(nsamples_all*1.0);//SPLIT
//    cout << ntrain_samples<<endl;
//    // Create or load MLP classifier
//    if( !filename_to_load.empty() )
//    {
//        // load MLP model
//        model = load_classifier<ANN_MLP>(filename_to_load);
//        if( model.empty() )
//            return false;
//        //ntrain_samples = 0;
//    }
//    else{
//        //create MLP model
//
//        Mat train_data = data.rowRange(0, ntrain_samples);
//        Mat train_responses = Mat::zeros( ntrain_samples, class_count, CV_32F );
////        cout << "train_data= " << endl << " " << train_data << endl << endl;
//
//        // 1. unroll the responses
//        cout << "Unrolling the responses...\n";
//        for( int i = 0; i < ntrain_samples; i++ )
//        {
//            int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
////            cout << "labels " << cls_label << endl;
//            train_responses.at<float>(i, cls_label) = 1.f;
//        }
//
//        // 2. train classifier
//        int layer_sz[] = { data.cols, 100,100,100, class_count };
////        int layer_sz[] = { data.cols, 32,64, 128, 256,512,256, 128, 64,32, class_count };//todo
////        int layer_sz[] = { data.cols,64, 128, 256,512,256, 128, 64, class_count };//todo
//        cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;//todo sizeof layer_sz 24 sizeof layer_sz[0]) 4
//        int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
//        cout << " nlayers  " << nlayers << endl;
//        Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );
//
//#if 1
//        int method = ANN_MLP::BACKPROP;
////        double method_param = 0.001;
////        double method_param = 0.0001;
//        double method_param = 0.000001;
//        int max_iter = 1000;
////        int max_iter = 500;
////        int max_iter = 300;
////        int max_iter = 100;
////        int max_iter = 50;
//#else
//        int method = ANN_MLP::RPROP;
//        double method_param = 0.1;
//        int max_iter = 1000;
//#endif
//
//        Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);
//
//        cout << "Training the classifier (may take a few minutes)...\n";
//        model = ANN_MLP::create();
//        model->setLayerSizes(layer_sizes);
//        model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0, 0);//sigmod
//        model->setTermCriteria(TC(max_iter,0));
//        model->setTrainMethod(method, method_param);
//        model->train(tdata);
//        cout << endl;
//    }
//    test_and_save_classifier(model, data, responses, ntrain_samples, 0, filename_to_save);
//    return true;
//}
//
//
//
