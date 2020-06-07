//
// Created by Tommy on 2020/6/7.
//

#include "LoadData.h"

LoadData::LoadData(char * filePath) {
    this->get_files_name(filePath);

    for (int i = 0; i < this->samples.size(); i++) {
        cout << "sample name:"<< samples[i].file_name<<", "
                << " label:"<< samples[i].label<<", "
                << " path:"<< samples[i].path<<", "
                << " feature:"<< samples[i].feature<<", "<<endl;
    }

}

LoadData::~LoadData() {

}

void LoadData::get_files_name(char * filePath ) {
//    char * filePath = "./images/test/";
    cout << "filePath:" <<filePath  <<endl;
    vector<string> files_name_full =  getFiles( filePath );
      for(int i=0; i< files_name_full.size(); ++i)
      {
//          cout << files_name_full[i] << endl;
          int index = files_name_full[i].find_first_of(".");
          string filename = files_name_full[i].substr (0,index);
//          cout <<"filename:" <<filename<< endl;
          vector<string> values; //
          split(filename,values,"_");
//          cout <<"values0:" <<values[0] <<"values1:"<< values[1]<< endl;
          Sample sample;
          sample.file_name = filename;
          sample.label = stringToNum<int>(values[0]);
          sample.path = filePath+files_name_full[i];
//              cout <<"  sample.path:" <<  sample.path<< endl;
          this->samples.push_back(sample);
      }
}

vector<string> LoadData:: getFiles(string cate_dir){
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