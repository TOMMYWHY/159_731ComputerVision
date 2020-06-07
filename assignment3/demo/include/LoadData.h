//
// Created by Tommy on 2020/6/7.
//

#ifndef GESTURE_LOADDATA_H
#define GESTURE_LOADDATA_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>


using namespace std;
template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}
struct Sample{
    int label;
    double feature;
    string file_name;
    string path;
};

class LoadData {
public:

    vector<Sample> samples;

    LoadData(char * filePath);
    ~LoadData();
    void get_files_name(char * filePath );
    vector<string> getFiles(string cate_dir);
    void split(const string &str,vector<string> &v,string spacer);
//    void save_feature();

};


#endif //GESTURE_LOADDATA_H
