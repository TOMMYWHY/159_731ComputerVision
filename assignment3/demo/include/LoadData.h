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
using namespace std;

struct Sample{
    int label;
    double feature;
    string file_name;
};

class LoadData {
public:
    LoadData();
    ~LoadData();
    vector<Sample> samples;
    void get_files_name();

//    void save_feature();

};


#endif //GESTURE_LOADDATA_H
