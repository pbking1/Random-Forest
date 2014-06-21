//
//  DecisionTree.cpp
//  Random-Forest
//
//  Created by Cho-Yeung Lam on 12/6/14.
//  Copyright (c) 2014 Cho-Yeung Lam. All rights reserved.
//

#include "DecisionTree.h"
#include <algorithm>
#include <math.h>

// Private methods

short DecisionTree::regLeaf_mode(vector<int> span)
{
    vector<int> histogram(NUM_CATEGORIES, 0);
    for (vector<int>::iterator iter = span.begin(); iter != span.end(); iter++) {
        histogram[dataSet[*iter * NUM_COLUMN]]++;
    }
    
    short label = 0;
    int maxcount = 0;
    for (size_t i = 0; i < NUM_CATEGORIES; i++)
        if (histogram.at(i) >= maxcount) {
            label = i;
            maxcount = histogram.at(i);
        }
    
    vector<int> maxVector;
    for (size_t i = 0; i < NUM_CATEGORIES; i++)
        if (histogram.at(i) == maxcount)
            maxVector.push_back((int)i);
    
    if (maxVector.size() > 1) {
        srand((unsigned)time(NULL));
        label = maxVector.at(rand() % (maxVector.size()-1));
    }
    
    labelCount[(int)label]++;
    return label;
}

double DecisionTree::Gini(vector<int> span)
{
    double result = 0;
    int labelCnt[NUM_CATEGORIES] = {0};
    short label = 0;
    for (vector<int>::iterator iter = span.begin(); iter != span.end(); iter++) {
        label = (short)dataSet[*iter * NUM_COLUMN];
        labelCnt[(int)label]++;
    }
    
    double totalCheck = 0;
    for (int label = 0; label < NUM_CATEGORIES; label++) {
        double percent = (double)labelCnt[label] / (double)span.size();
        result += percent * percent;
        totalCheck += percent;
    }
    result = 1.0 - result;
    return result;
}

void DecisionTree::chooseBestSplit(vector<int> span, set<int> features, int &bestIndex, short &bestValue)
{
//    cout << "[trainFeatures size] " << features.size() << endl;
    size_t counter = 1;
    short sameVal = dataSet[span.at(0) * NUM_COLUMN];
    for (size_t cnt = 1; cnt < span.size(); cnt++)
        if (dataSet[span.at(cnt) * NUM_COLUMN] == sameVal) counter++;
    if (counter == span.size()) {
        bestIndex = -1;
        bestValue = sameVal;
        return;
    }
    
    double G = Gini(span);
    double bestG = INFINITY;
    vector<int> lSpan, rSpan;
    
    int outloop = 0, inloop = 0;
    for (set<int>::iterator feature = features.begin(); feature != features.end(); feature++) {
        
        // Optimization[1]: vector -> set
        set<short> valueSet;
        for (vector<int>::iterator iter = span.begin(); iter != span.end(); iter++) {
            valueSet.insert(dataSet[*iter * NUM_COLUMN + *feature]);
        }
        
        for (set<short>::iterator iter = valueSet.begin(); iter != valueSet.end(); iter++) {
            binSplitData(span, lSpan, rSpan, *feature, *iter); // Fixbug
            
            if (lSpan.size() < tolN || rSpan.size() < tolN) {
                continue;
            }
            double newG = ((double)lSpan.size() / (double)span.size()) * Gini(lSpan) + ((double)rSpan.size() / (double)span.size()) * Gini(rSpan);
            if (newG < bestG) {
                bestIndex = *feature;
                bestValue = dataSet[*iter * NUM_COLUMN + *feature];
                bestG = newG;
            }
            inloop++;
        }
        
        valueSet.clear();
        outloop++;
    }
    
    if (G - bestG < tolS) {
        bestIndex = -1, bestValue = regLeaf_mode(span);
        return;
    }
    binSplitData(span, lSpan, rSpan, bestIndex, bestValue);
    if (lSpan.size() < tolN || rSpan.size() < tolN) {
        bestIndex = -1, bestValue = regLeaf_mode(span);
        return;
    }
    else {
        /*already stored bestIndex and bestValue*/
    }
}

void DecisionTree::binSplitData(vector<int> pSpan, vector<int> &lSpan, vector<int> &rSpan, int feature, short value)
{
    lSpan.clear(), rSpan.clear();
    vector<int>::iterator it;
    for (it = pSpan.begin(); it != pSpan.end(); it++) {
        if (dataSet[*it * NUM_COLUMN + feature] <= value)
            lSpan.push_back(*it);
        else
            rSpan.push_back(*it);
    }
}

void DecisionTree::recursive_create_tree(vector<int> span, Node* &subroot, set<int> features)
{
    static int node_count = 0;
    int bestIndex = 0;
    short bestValue = 0;
    chooseBestSplit(span, features, bestIndex, bestValue);
    
    subroot = new Node(bestIndex, bestValue);
    // No bestIndex: Leaf
    if (bestIndex == -1) {
        return;
    }
    
    features.erase(bestIndex);
    
    node_count++;
    featureChosen[bestIndex] = true;
    
    vector<int> lSpan, rSpan;
    binSplitData(span, lSpan, rSpan, bestIndex, bestValue);
    recursive_create_tree(lSpan, subroot->left, features);
    recursive_create_tree(rSpan, subroot->right, features);
}

// Public methods

DecisionTree::DecisionTree(short *dataSet)
{
    this->dataSet = dataSet;
    featureChosen[0] = true; // The labels
    for (int cnt = 1; cnt < NUM_COLUMN; cnt++)
        featureChosen[cnt] = false;
    
    // Random m = √p features(aka 56 in this case)
    int rand_feature = 0;
    srand((unsigned)time(NULL));
    int insertCnt = 0;
    
    while (trainFeatures.size() <= NUM_TRAIN_FEATURES) {
        rand_feature = (rand() % (NUM_COLUMN-1)) + 1;
        trainFeatures.insert(rand_feature);
        insertCnt++;
    }
    
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        labelCount[i] = 0;
    }
}

void DecisionTree::createTree()
{
    // Recursive begin with the root Node
    // Random 10000 rows
    int modified_num_row = (int)(NUM_ROW * 2.0 / 3.0);
    set<int> wholeSpan;
    srand((unsigned)time(NULL));
    int rand_row = 0;
    while (wholeSpan.size() <= modified_num_row) {
        rand_row = rand() % NUM_ROW;
        wholeSpan.insert(rand_row);
    }
    
    vector<int> wholeSpan_vec;
    for (set<int>::iterator iter = wholeSpan.begin(); iter != wholeSpan.end(); iter++) {
        wholeSpan_vec.insert(wholeSpan_vec.end(), *iter);
    }
    
    recursive_create_tree(wholeSpan_vec, this->root, trainFeatures);
    
    cout << "=================LABEL OF TREE================" << endl;
    int totalLeaves = 0;
    for (int label = 0; label < NUM_CATEGORIES; label++) {
        cout << "[" << label << "] " << labelCount[label] << endl;
        totalLeaves += labelCount[label];
    }
    cout << "[Leaves Count] " << totalLeaves << endl;
}

void DecisionTree::clearData()
{
    trainRows.clear();
    trainFeatures.clear();
}

Node* DecisionTree::getRoot()
{
    return this->root;
}
