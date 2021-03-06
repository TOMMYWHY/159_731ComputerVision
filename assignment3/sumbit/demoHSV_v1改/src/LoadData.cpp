//
// Created by Tommy on 2020/6/7.
//

#include "LoadData.h"

/*for test single img*/
LoadData::LoadData(Mat imageROI) {

    this->test_ce =  get_single_feature_HSV(imageROI);
//    this->test_ce =  get_single_feature_RGB(imageROI);

}

vector<double> LoadData:: get_single_feature_HSV(Mat imageROI){
//    medianBlur(imageROI, imageROI, 5);
//    printf("1111");
//    imshow( "imageROI" , imageROI ) ;
    Mat binary_img,element;
    Mat hsv ( imageROI.rows , imageROI.cols ,CV_8UC3 ,Scalar::all(0)) ;
    cvtColor(imageROI, hsv, COLOR_BGR2HSV);
    inRange(hsv,Scalar(0,66,76),Scalar(179,255,255),binary_img);
//    inRange(hsv,Scalar(0,29,81),Scalar(179,255,242),binary_img);
//iLowH:0iLowS:66iLowV:76iHighH:179iHighS:255iHighV:255
    element=getStructuringElement(MORPH_RECT,Size(3,3));
    morphologyEx(binary_img,binary_img,MORPH_OPEN,element);
    morphologyEx(binary_img,binary_img,MORPH_CLOSE,element);
    Canny(binary_img,binary_img,20,60);
//    imshow("binary_img",binary_img);

    vector<vector<Point> > contours ;
    findContours ( binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
//    findContours ( obj_binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
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
    vector<float> CE;
    vector<double> feature_values = this->EllipticFourierDescriptors(contours[largestcontour], CE);
    return feature_values;
}

vector<double> LoadData:: get_single_feature_RGB(Mat imageROI){
    //    medianBlur(imageROI, imageROI, 3);
    Mat hsv ( imageROI.rows , imageROI.cols ,CV_8UC3 ,Scalar::all(0)) ;
    cvtColor(imageROI, hsv, COLOR_BGR2HSV);
    imshow( "hsv" , hsv ) ;
    vector<Mat> channels;
    ::split(hsv, channels);//分割image1的通道
    Mat channelsH = channels[0];//获取通道1
    Mat  channelsS = channels[1];//获取通道2
    Mat channelsV = channels[2];
    imshow( "channelsH" , channelsH ) ;
    imshow( "channelsS" , channelsS ) ;
    imshow( "channelsV" , channelsV ) ;

    Mat gray_img, binary_img,obj_img;
    threshold ( channelsS , binary_img , 70 , 255  , THRESH_BINARY ) ;//todo 区域划分不精准
    imshow( "binary_img" , binary_img ) ;

    // todo

    vector<vector<Point> > contours ;
    findContours ( binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
//    findContours ( obj_binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
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
    vector<float> CE;
    vector<double> feature_values = EllipticFourierDescriptors(contours[largestcontour], CE);
    return feature_values;
}

/*for train images*/

LoadData::LoadData(string filePath, string dataFrame_path, int model) {
    this->filePath = filePath;
    this->dataFrame_path = dataFrame_path;
    cout << this->filePath<< endl;
    cout << "extracting features..."<<endl;
    this->get_files_name();
//    this->get_features();
    if(model==0){
        this->get_features_RGB(); // trianset
    } else if(model ==1){
        this->get_features(); // testset
    }
//    this->get_features_RGB();
    this->save_feature();



}

/*read files name*/
void LoadData::get_files_name() {
//    cout << "filePath:" <<this->filePath  <<endl;
    vector<string> files_name_full =  getFiles( this->filePath );
//    cout <<files_name_full.size()<<endl;
    for(int i = 0; i < files_name_full.size(); i++)
//    for(int i = 0; i < 1; i++)
      {
//          cout << "files_name_full:" <<files_name_full[i] <<endl;
          int index = files_name_full[i].find_first_of(".");
          string filename = files_name_full[i].substr (0,index);
          vector<string> values; //
          split(filename,values,"_");
          Sample sample;
          sample.file_name = filename;
//          cout << "values[0]:"<<values[0] <<"; values[1]"<<values[1]<<endl;
//          cout << "values[0].length:"<<values[0].length()<<endl;
          if(values[0].length()<2){
            sample.label = values[0];
          }else{
              sample.label = values[1];
          }
        sample.path = filePath+files_name_full[i];
          this->samples.push_back(sample);
      }
}

void LoadData::get_features() {
    for (int i = 0; i < this->samples.size(); i++)
//        for (int i = 0; i <2; i++)
    {
        Mat img = imread( samples[i].path,1);

        samples[i].feature =  get_single_feature_HSV(img);
    }

}
void LoadData::get_features_RGB() {
    for (int i = 0; i < this->samples.size(); i++)
//        for (int i = 0; i <2; i++)
    {
        Mat img = imread( samples[i].path,1);
        Mat gray_img,binary_img;
        cvtColor( img , gray_img , COLOR_BGR2GRAY );
        threshold ( gray_img , binary_img , 5 , 255 , THRESH_BINARY ) ;
        imshow("binary_img" , binary_img ) ;
        vector<vector<Point> > contours ;
        findContours ( binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;
        Mat drawing = Mat::zeros( binary_img.size(), CV_8UC3 );
        Scalar color = CV_RGB( 0, 255,0 );
        int largestcontour=0;
        long int largestsize=0;
        vector< vector<Point> > filterContours;	//
        vector< Point > hull;	//
        for(int i = 0; i< contours.size(); i++ ) {
            if(largestsize < contours[i].size()) {
                largestsize=contours [ i ]. size () ;
                largestcontour=i ;
            }

        }
        drawContours( drawing, contours, largestcontour, color, 1, 8);
        imshow("drawing" , drawing ) ;
        vector<float> CE;
        vector<double> feature_values = EllipticFourierDescriptors(contours[largestcontour], CE);
        samples[i].feature = feature_values;
    }

}

void LoadData::save_feature() {
    ofstream w_file;
//    w_file.open("./res/test.data",ios::app);
    w_file.open(this->dataFrame_path);
//    cout<<"save_feature"<<endl;
    for (int i = 0; i < samples.size(); i++)
//    for (int i = 0; i < 3; i++)
    {
        w_file <<samples[i].label <<",";
        cout<< samples[i].label <<"," ;
        for (int j = 0; j < samples[i].feature.size(); j++) {
            cout<< samples[i].feature[j] <<"," ;
            w_file <<samples[i].feature[j] <<",";
        }
        cout <<endl;
        w_file<<endl;
    }
    w_file.close();
    cout << endl;
    cout << "dataframe saved at : "<<this->dataFrame_path  <<endl;
}



vector<double> LoadData::get_single_obj_feature(Mat & obj_img){
    imshow("obj_img111" , obj_img ) ;
    Mat obj_gray_img,obj_binary_img;
    cvtColor( obj_img , obj_gray_img , COLOR_BGR2GRAY );
    imshow("obj_gray_img" , obj_gray_img ) ;

    threshold ( obj_gray_img , obj_binary_img , 5 , 255 , THRESH_BINARY ) ;
//    imshow("obj_binary_img" , obj_binary_img ) ;

    vector<vector<Point> > contours ;
    findContours ( obj_binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_NONE) ;

    Mat drawing = Mat::zeros( obj_binary_img.size(), CV_8UC3 );
    Scalar color = CV_RGB( 0, 255,0 );
    int largestcontour=0;
    long int largestsize=0;
    vector< vector<Point> > filterContours;	//
    vector< Point > hull;	//
    for(int i = 0; i< contours.size(); i++ ) {
        if(largestsize < contours[i].size()) {
            largestsize=contours [ i ]. size () ;
            largestcontour=i ;
        }

    }
    drawContours( drawing, contours, largestcontour, color, 1, 8);
//    imshow("drawing" , drawing ) ;

    vector<float> CE;
    vector<double> feature_values = EllipticFourierDescriptors(contours[largestcontour], CE);
    return feature_values;
}

vector<double> LoadData::EllipticFourierDescriptors(vector<Point>& contour , vector< float> & CE) {
    vector<float> ax, ay, bx, by;
//    vector<Feature> feature_list;
    vector<double> feature_values;
    int m = contour.size();

//    int n = 30;//number of CEs we are interested in computing
    int n = 20;//number of CEs we are interested in computing
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
    for (int count = 0; count < n && count < CE.size(); count++) {
//        printf("%d CE %f ax %f ay %f bx %f by%f \n" ,count, CE[count], ax[count], ay[count], bx[count], by[count] );
        feature_values.push_back(CE[count]);
    }
    return feature_values;

}


LoadData::~LoadData() {

}

/*====*/
vector<string> LoadData:: getFiles(string cate_dir){
    vector<string> files;//存放文件名
//    cout << "cate_dir:" <<cate_dir <<endl;
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

void LoadData:: split(const string &str,vector<string> &v,string spacer)
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


