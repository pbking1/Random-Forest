//
//  FileReader.cpp
//  Random-Forest
//
//  Created by Cho-Yeung Lam on 12/6/14.
//  Copyright (c) 2014 Cho-Yeung Lam. All rights reserved.
//

#include "FileReader.h"
#include <cmath>

#define NUM_INFILE 4

// Private methods

void FileReader::split(const string& src, const string& delim)
{
    if (src == "") return;
    
    int rowNum;
    string str = src;
    string::size_type start = 0, index;
    string id_num, user_id, item_id, rating;
    string valueStr;
    
    index = str.find_first_of(delim, start);
    id_num = str.substr(start, index-start);
    rowNum = atof(id_num.c_str()) - 1;
    start = str.find_first_not_of(delim, index);
    
    for (int cnt = 0; cnt < NUM_COLUMN-1; cnt++) {
        index = str.find_first_of(delim, start);
        valueStr = str.substr(start, index-start);
        double value = atof(valueStr.c_str());
        dataSet[rowNum * NUM_COLUMN + cnt] = value;
        start = str.find_first_not_of(delim, index);
    }
    
    index = str.find_first_of("\r", start);
    valueStr = str.substr(start, index-start);
    double value = atof(valueStr.c_str());
    dataSet[rowNum * NUM_COLUMN + NUM_COLUMN-1] = value;
    
//    io_mutex.lock();
    cout << rowNum+1 << " is loaded..." << endl;
//    io_mutex.unlock();
}

void FileReader::readSingleFile(string filePath)
{
    int rowCnt = 0;
    ifstream infile(filePath);
    string word;
    string delim(",");
    string textline;
    if(infile.good())
    {
        while(!infile.fail())
        {
            getline(infile, textline);
            split(textline, delim);
            rowCnt++;
            if (rowCnt >= row_eachfile) {
                infile.close();
                return;
            }
        }
    }
    infile.close();
}

// Public methods

FileReader::FileReader()
{
    size_t nSize = NUM_ROW * NUM_COLUMN * sizeof(double);
    char *pBuf = new char[nSize];
    dataSet = (double *)pBuf;
}

void FileReader::readFileList(string dirPath, int thresold)
{
    if (thresold != -1)
        this->row_eachfile = thresold;
    else
        this->row_eachfile = INFINITY;
    
    thread thrd[NUM_THREAD];
    for (int cnt = 1; cnt <= NUM_THREAD; cnt++) {
        ostringstream convert;
        convert << cnt;
        convert.str();
        string fileName = "/Users/apple/Developer/Random-Forest/Random-Forest/train" + convert.str() + ".csv";
        thrd[cnt-1] = thread(bind(&FileReader::readSingleFile, this, fileName));
    }
    
    for (int cnt = 1; cnt <= NUM_THREAD; cnt++)
        thrd[cnt-1].join();
}
