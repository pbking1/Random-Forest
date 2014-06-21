//
//  main.cpp
//  binary_search_tree
//
//  Created by ¡÷ ≥ı—Ù on 12-12-13.
//  Copyright (c) 2012ƒÍ __MyCompanyName__. All rights reserved.
//

#include "FileReader.h"
#include "DecisionTree.h"
#include "Constants.h"
#include "Tester.h"
#include "FileWriter.h"
#include <functional>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>
using namespace std;

int *votes;
mutex io_mutex;
int treeCounter;
int cateCnt[NUM_CATEGORIES];

void forest_per_thread(short *trainDataSet, short *testDataSet)
{
    Tester tester;
    tester.pointToData(testDataSet);
    
    for (int i = 0; i < NUM_TREES; i++)
    {
        DecisionTree dtree(trainDataSet);
        dtree.createTree();
        tester.changeRoot(dtree.getRoot());
        
        for (int row = 0; row < NUM_TEST_ROW; row++) {
            int row_result = tester.testResult(row);
            votes[row * NUM_CATEGORIES + row_result] += 1;
            cateCnt[row_result] += 1;
        }
        
        io_mutex.lock();
        cout << "Tree " << treeCounter++ << " finished testing!" << endl;

        
        cout << "=============20 Ex Case==============" << endl;
        for (int i = 0; i < 10; i++) {
            cout << "[" << i << "] ";
        }
        cout << endl;
        for (int i = 0; i < 20; i++) {
            cout << " ";
            for (int j = 0; j < NUM_CATEGORIES; j++) {
                cout << votes[i*NUM_CATEGORIES+j] << "  ";
            }
            cout << endl;
        }
        cout << endl;
        
//        cout << "=============CAT TOTAL==============" << endl;
//        for (int j = 0; j < NUM_CATEGORIES; j++) {
////            cateCnt[j] += tester.cateCount[j];
//            cout << "[" << j << "] " << cateCnt[j] << endl;
//        }
        io_mutex.unlock();
        
        dtree.clearData();
        tester.clearCateCount();
    }
}

int main(int argc, const char * argv[])
{
    size_t nSize = NUM_TEST_ROW * NUM_CATEGORIES * sizeof(int);
    char *pBuf = new char[nSize];
    votes = (int *)pBuf;
    
    for (int cat = 0; cat < NUM_CATEGORIES; cat++) {
        cateCnt[cat] = 0;
    }
    
    // Reading with multithreading under the hood
    FileReader trainreader(NUM_ROW, NUM_COLUMN);
    trainreader.readFileList("/Users/apple/Developer/Random-Forest/Random-Forest/train", -1);
    FileReader testreader(NUM_TEST_ROW, NUM_COLUMN-1);
    testreader.readFileList("/Users/apple/Developer/Random-Forest/Random-Forest/test0", -1);
    
    
    // Multithreading calculation
    treeCounter = 0;
    thread thrd[NUM_THREAD];
    for (int cnt = 0; cnt < NUM_THREAD; cnt++)
        thrd[cnt] = thread(bind(&forest_per_thread, trainreader.dataSet, testreader.dataSet));
    
    for (int cnt = 0; cnt < NUM_THREAD; cnt++)
        thrd[cnt].join();
    
    FileWriter writer;
    writer.writeResult(votes, "/Users/apple/Developer/Random-Forest/Random-Forest/out.csv");
    
    return 0;
}
