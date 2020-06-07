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


#include "LoadData.h"


using namespace std;
using namespace cv;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar *) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)*((image).channels())+2]



#if 001

vector<double> EllipticFourierDescriptors(vector<Point>& contour , vector< float> CE) {
    vector<float> ax, ay, bx, by;
    vector<double> feature_values;
    int m = contour.size();
    cout <<"contour.size:" << m <<endl;
    int n = 20;//number of CEs we are interested in computing
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
        printf("%d CE %f ax %f ay %f bx %f by%f \n" ,count, CE[count], ax[count], ay[count], bx[count], by[count] );
//        cout <<"CE index:"<<count <<"; CE value:"<< CE[count] <<endl;
        Feature feature;
//        feature.index = count;
//        feature.value = CE[count];
//        feature_list.push_back(feature);
        feature_values.push_back(CE[count]);

    }

    return feature_values;

}

Mat get_obj_img(Mat org_img){
    Mat gray_img, binary_img,obj_img;
    blur(org_img,org_img, Size(3,3));
    cvtColor ( org_img , gray_img , COLOR_BGR2GRAY ) ;
    threshold ( gray_img , binary_img , 225 , 255 , THRESH_BINARY ) ;//todo 区域划分不精准
    //    imshow("Binary",binary_img);

    Mat reverse_img = 255 - binary_img;
    org_img.copyTo(obj_img);
    for (int x = 0; x < obj_img.cols; x++) {
        for (int y = 0; y < obj_img.rows; y++) {
            if(int(Mpixel(reverse_img ,x,y))==0){
                MpixelB(obj_img ,x,y)= 0;
                MpixelG(obj_img ,x,y)= 0;
                MpixelR(obj_img ,x,y)= 0;
            }
        }
    }
//    imshow("obj_img",obj_img);
    return obj_img;
}

vector<double> get_feature(Mat obj_img){
    Mat obj_gray_img,obj_binary_img;
    cvtColor( obj_img , obj_gray_img , COLOR_BGR2GRAY );
    threshold ( obj_gray_img , obj_binary_img , 5 , 255 , THRESH_BINARY ) ;
    vector<vector<Point> > contours ;
    findContours ( obj_binary_img , contours ,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE) ;
    Mat drawing = Mat::zeros( obj_binary_img.size(), CV_8UC3 );
    Scalar color = CV_RGB( 0, 255,0 );
    int largestcontour=0;
    long int largestsize=0;
    vector< vector<Point> > filterContours;	// 筛选后的轮廓
    vector< Point > hull;	// 凸包络的点集

    for(int i = 0; i< contours.size(); i++ ) {

        if (fabs(contourArea(Mat(contours[i]))) > 30000)	//判断手进入区域的阈值
        {
            filterContours.push_back(contours[i]);
        }

    }
    drawContours(drawing, filterContours, -1, Scalar(0,0,255), 1);
    imshow("drawing" , drawing ) ;
    vector<float> CE;
    vector<double> feature_values = EllipticFourierDescriptors(contours[largestcontour], CE);
    return feature_values;
}

int main (){

//    LoadData test("./images/test/","./res/test.data");
    LoadData all_img("./images/all_img/","./res/all_img.data");

    cout <<"write done" << endl;
    waitKey(0);

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