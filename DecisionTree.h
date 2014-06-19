//
//  DecisionTree.h
//  Random-Forest
//
//  Created by Cho-Yeung Lam on 12/6/14.
//  Copyright (c) 2014 Cho-Yeung Lam. All rights reserved.
//

#ifndef __Random_Forest__DecisionTree__
#define __Random_Forest__DecisionTree__

#include <iostream>
#include <vector>
#include <set>
#include "Constants.h"
using namespace std;

#define tolS 0.001
#define tolN 3

struct Node {
    short spInd;
    short spVal;
    Node *left;
    Node *right;
    Node(short _spInd, short _spVal, Node *l = NULL, Node *r = NULL) {
        spInd = _spInd;
        spVal = _spVal;
        left = l;
        right = r;
    }
};

class DecisionTree
{
private:
    Node *root;
    short *dataSet;
    bool featureChosen[NUM_COLUMN]; // TODO: fix it to map
    set<int> trainRows;  // TODO: Extendability
    set<int> trainFeatures; // TODO: Extendability
    /* Should suit dataSet of different size */
    
    void binSplitData(vector<int> pSpan, vector<int> &lSpan, vector<int> &rSpan, int feature, short value);
    void chooseBestSplit(vector<int> span, int &bestIndex, short &bestValue);
    void recursive_create_tree(vector<int> span, Node* &subroot);
    short regLeaf_mode(vector<int> span);
    double Gini(vector<int> span);
    
    // Var only for logging
    int labelCount[NUM_CATEGORIES];
public:
    DecisionTree(short *dataSet = NULL);
    void createTree();
    void clearData();
    Node *getRoot();
};

#endif /* defined(__Random_Forest__DecisionTree__) */
