/*
++++++++++++++++++++++++++++++++++++++++++++++++++
author: kevinhikali
email:  hmingwei@gmail.com
intro:  This is Kevin.H CPP lib program
++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef __C_PP_LIB_READ_FILENAME_H
#define __C_PP_LIB_READ_FILENAME_H

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

// C++
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

namespace KEVINHIKALI {

vector<string> readFileList(string path)
{
        DIR *dir;

        int len = path.length();
        char pathCArr[len+1];
        memset(pathCArr, '\0', sizeof(pathCArr));
        for (int i=0;i<sizeof(pathCArr);i++) {
                pathCArr[i] = path[i];
        }
        if ((dir=opendir(pathCArr)) == NULL) {
                cerr << "In cpp_lib_read_filename, readFileList function, open dir error, addr = ";
                cerr << path << endl;
                exit(1);
        }

        vector<string> filename;
        struct dirent *ptr;
        char base[1000];
        while ((ptr=readdir(dir)) != NULL)
        {
                // current dir OR parrent dir
                if (strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0) {
                        continue;
                } else if (ptr->d_type == 8) { // file
                        filename.push_back(ptr->d_name);
                } else if (ptr->d_type == 10) {  // link file
                        filename.push_back(ptr->d_name);
                } else if (ptr->d_type == 4) { // dir
                        memset(base,'\0',sizeof(base));
                        strcpy(base, pathCArr);
                        strcat(base,"/");
                        strcat(base,ptr->d_name);
                        readFileList(base);
                }
        }
        closedir(dir);
        return filename;
}

vector<string> readFolderList(string path)
{
        vector<string> folder_names;

        int len = path.length();
        char basePath[len+1];
        memset(basePath, '\0', sizeof(basePath));
        for (int i=0;i<sizeof(basePath);i++) {
                basePath[i] = path[i];
        }

        DIR *dir;
        struct dirent *ptr;
        char base[1000];

        if ((dir=opendir(basePath)) == NULL)
        {
                perror("Open dir error...");
                exit(1);
        }

        while ((ptr=readdir(dir)) != NULL)
        {
                if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
                        continue;
                //        else if(ptr->d_type == 8)    ///file
                ////            printf("d_name:%s/%s\n",basePath,ptr->d_name);
                //            folder_names.push_back(ptr->d_name);
                //        else if(ptr->d_type == 10)    ///link file
                ////            printf("d_name:%s/%s\n",basePath,ptr->d_name);
                //            folder_names.push_back(ptr->d_name);
                else if(ptr->d_type == 4)    ///dir
                {
                        folder_names.push_back(ptr->d_name);
                        //            memset(base,'\0',sizeof(base));
                        //            strcpy(base,basePath);
                        //            strcat(base,"/");
                        //            strcat(base,ptr->d_name);
                        //            readFolderList(base);
                }
        }
        closedir(dir);

        return folder_names;
}

bool string_int_comp(const string &a,const string &b)
{
        stringstream ss;
        ss<<a;
        int aInt;
        ss>>aInt;

        ss.str("");
        ss.clear();

        ss<<b;
        int bInt;
        ss>>bInt;

        return aInt<bInt;
}

void sortFilenames(vector<string>& filenames)
{
        sort(filenames.begin(), filenames.end(), string_int_comp);
}

}

#endif
