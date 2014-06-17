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
#include <iostream>
#include <string>
using namespace std;

int *votes;
DecisionTree trees[NUM_TREES];

int main(int argc, const char * argv[])
{
    size_t nSize = NUM_TEST_ROW * NUM_CATEGORIES * sizeof(int);
    char *pBuf = new char[nSize];
    votes = (int *)pBuf;
    
    
    FileReader reader;
    reader.readFileList("/Users/apple/Developer/Random-Forest/Random-Forest/", -1);
    Tester tester("/Users/apple/Developer/Random-Forest/Random-Forest/test.csv", "/Users/apple/Developer/Random-Forest/Random-Forest/out.csv");
    
    for (int i = 0; i < NUM_TREES; i++) {
        trees[i] = DecisionTree(reader.dataSet);
        trees[i].createTree();
        tester.changeRoot(trees[i].getRoot());
        tester.begin();
        for (int row = 0; row < NUM_TEST_ROW; row++) {
            votes[row * NUM_CATEGORIES + tester.result[row]] = tester.result[row];
        }
        cout << "Tree " << i+1 << " finished!" << endl;
        tester.clearCateCount();
    }
    
    tester.writeResult(votes);
    
    return 0;
}
