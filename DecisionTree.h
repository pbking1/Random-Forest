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
#define tolN 4

struct Node {
    double spInd;
    double spVal;
    Node *left;
    Node *right;
    Node(double _spInd, double _spVal, Node *l = NULL, Node *r = NULL) {
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
    double *dataSet;
    bool featureChosen[NUM_COLUMN]; // TODO: fix it to map
    set<int> trainRows;  // TODO: Extendability
    set<int> trainFeatures; // TODO: Extendability
    /* Should suit dataSet of different size */
    
    void binSplitData(vector<int> pSpan, vector<int> &lSpan, vector<int> &rSpan, int feature, double value);
    void chooseBestSplit(vector<int> span, int &bestIndex, double &bestValue);
    void recursive_create_tree(vector<int> span, Node* &subroot);
    double regLeaf_mean(vector<int> span);
    double regLeaf_mode(vector<int> span);
    double Gini(vector<int> span);
    
    
    int labelCount[NUM_CATEGORIES];
public:
    DecisionTree(double *dataSet = NULL);
    void createTree();
    Node *getRoot();
};

#endif /* defined(__Random_Forest__DecisionTree__) */
