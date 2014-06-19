//
//  FileReader.h
//  Random-Forest
//
//  Created by Cho-Yeung Lam on 12/6/14.
//  Copyright (c) 2014 Cho-Yeung Lam. All rights reserved.
//

#ifndef __Random_Forest__FileReader__
#define __Random_Forest__FileReader__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <functional>
#include <thread>
#include <mutex>
#include "Constants.h"
using namespace std;

class FileReader
{
private:
    void readSingleFile(string filePath);
    mutex io_mutex;
    int row_eachfile;
    int rowCnt, colCnt;
public:
    FileReader(int rowCnt = 0, int colCnt = 0);
    void split(const string& src, const string& delim);
    short *dataSet;
    void readFileList(string dirPath, int thresold);
    /* -1 for reading the whole file*/
};

#endif /* defined(__Random_Forest__FileReader__) */
