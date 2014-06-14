//
//  DecisionTree.cpp
//  Random-Forest
//
//  Created by Cho-Yeung Lam on 12/6/14.
//  Copyright (c) 2014 Cho-Yeung Lam. All rights reserved.
//

#include "DecisionTree.h"
#include <algorithm>
#include <set>
#include <cmath>

// Private methods
double DecisionTree::regLeaf(vector<int> span)
{
    vector<double> allLabel;
    vector<int>::iterator iter;
    for (iter = span.begin(); iter != span.end(); iter++) {
        allLabel.push_back(dataSet[*iter * NUM_COLUMN]);
    }
    sort(allLabel.begin(), allLabel.end());
    
    double mean = 0;
    size_t total = allLabel.size();
    if (total % 2 == 1)
        mean = allLabel.at(total/2+1);
    else
        mean = (allLabel.at(total/2-1) + allLabel.at(total/2)) / 2.0;
    
    return mean;
}

double DecisionTree::regErr(vector<int> span)
{
    // Switch to Gini afterward
    double variance = 0;
    double average = 0;
    vector<int>::iterator iter;
    for (iter = span.begin(); iter != span.end(); iter++)
        average += dataSet[*iter * NUM_COLUMN];
    
    average = average / (double)span.size();
    for (iter = span.begin(); iter != span.end(); iter++)
        variance += (dataSet[*iter * NUM_COLUMN] - average) * (dataSet[*iter * NUM_COLUMN] - average);
    
    variance = variance / (double)span.size();
    return variance;
}

void DecisionTree::chooseBestSplit(vector<int> span, int &bestIndex, double &bestValue)
{
    size_t counter = 1;
    double sameVal = dataSet[span.at(0) * NUM_COLUMN];
    for (size_t cnt = 1; cnt < span.size(); cnt++)
        if (dataSet[span.at(cnt) * NUM_COLUMN] == sameVal) counter++;
    if (counter == span.size()) {
        bestIndex = -1;
        bestValue = sameVal;
        return;
    }
    
    double S = regErr(span);
    double bestS = INFINITY;
    double newS = INFINITY;
    vector<int> lSpan, rSpan;
    
    for (int feature = 1; feature < NUM_COLUMN; feature++) {
        
        // Optimization[1]: vector -> set
        set<int> valueSet;
        for (vector<int>::iterator iter = span.begin(); iter != span.end(); iter++) {
            valueSet.insert(*iter);
        }
        
        for (set<int>::iterator iter = valueSet.begin(); iter != valueSet.end(); iter++) {
            binSplitData(span, lSpan, rSpan, feature, dataSet[*iter * NUM_COLUMN + feature]);
            
            if (lSpan.size() < tolN || rSpan.size() < tolN) continue;
            newS = regErr(lSpan) + regErr(rSpan);
            if (newS < bestS) {
                bestIndex = feature;
                bestValue = dataSet[*iter * NUM_COLUMN + feature];
            }
        }
    }
    
    if (S - bestS < tolS)
        bestIndex = -1, bestValue = regLeaf(span);
    binSplitData(span, lSpan, rSpan, bestIndex, bestValue);
    if (lSpan.size() < tolN || rSpan.size() < tolS)
        bestIndex = -1, bestValue = regLeaf(span);
    else { /*already stored bestIndex and bestValue*/ }
}

void DecisionTree::binSplitData(vector<int> pSpan, vector<int> &lSpan, vector<int> &rSpan, int feature, double value)
{
    lSpan.clear(), rSpan.clear();
    vector<int>::iterator it;
    for (it = pSpan.begin(); it != pSpan.end(); it++) {
        if (dataSet[*it * NUM_COLUMN + feature] > value)
            lSpan.push_back(*it);
        else
            rSpan.push_back(*it);
    }
}

void DecisionTree::recursive_create_tree(vector<int> span, Node* &subroot)
{
    int bestIndex = 0;
    double bestValue = 0;
    chooseBestSplit(span, bestIndex, bestValue);
    // No bestIndex: Leaf
    if (bestIndex == -1) return;
    
    subroot = new Node(bestIndex, bestValue);
    
    vector<int> lSpan, rSpan;
    binSplitData(span, lSpan, rSpan, bestIndex, bestValue);
    recursive_create_tree(lSpan, subroot->left);
    recursive_create_tree(rSpan, subroot->right);
}

// Public methods

DecisionTree::DecisionTree(double *dataSet)
{
    this->dataSet = dataSet;
}

void DecisionTree::createTree()
{
    // Recursive begin with the root Node
    vector<int> wholeSpan;
    for (int i = 0; i < NUM_ROW; i++) {
        wholeSpan.push_back(i);
    }
    
    recursive_create_tree(wholeSpan, this->root);
}

Node* DecisionTree::getRoot()
{
    return this->root;
}
