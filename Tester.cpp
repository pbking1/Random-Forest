//
//  Tester.cpp
//  Random-Forest
//
//  Created by Cho-Yeung Lam on 16/6/14.
//  Copyright (c) 2014 Cho-Yeung Lam. All rights reserved.
//

#include "Tester.h"

// Private methods

void Tester::testResult(int rowNum)
{
    // 写入result[rowNum-1]
    int category_out = 0;
    Node *rootMark = this->root;
    Node *cur = this->root;
    int spIndex = 0;
    int spValue = 0;
    
    while ((cur->left != NULL) || (cur->right != NULL)) {
        spIndex = cur->spInd;
        spValue = cur->spVal;
        if (temp_values[spIndex-1] <= spValue)
            cur = cur->left;
        else
            cur = cur->right;
    }
    
    category_out = cur->spVal;
    this->result[rowNum] = category_out;
    
    cateCount[category_out]++;
//    cout << "[CATEGORY RESULT] " << category_out << endl; 
    this->root = rootMark;
}

void Tester::split(const string &src, const string &delim)
{
    if (src == "") return;
    
    int rowNum;
    string str = src;
    string::size_type start = 0, index;
    string id_num, user_id, item_id, rating;
    string valueStr;
    
    index = str.find_first_of(delim, start);
    id_num = str.substr(start, index-start);
    if (id_num == "id") return;
    
    rowNum = atof(id_num.c_str()) - 1;
    start = str.find_first_not_of(delim, index);
    
    
    if (rowNum == -1) {
        cout << src << endl;
    }
    
    for (int cnt = 0; cnt < NUM_COLUMN-2; cnt++) {
        index = str.find_first_of(delim, start);
        valueStr = str.substr(start, index-start);
        double value = atof(valueStr.c_str());
        temp_values[cnt] = value;
        start = str.find_first_not_of(delim, index);
    }
    
    index = str.find_first_of("\r", start);
    valueStr = str.substr(start, index-start);
    double value = atof(valueStr.c_str());
    temp_values[NUM_COLUMN-2] = value;
    
    testResult(rowNum);
}

// Public methods

Tester::Tester(string testFilePath, string resultFilePath, Node *root)
{
    this->testFilePath = testFilePath;
    this->resultFilePath = resultFilePath;
    this->root = root;
}

void Tester::changeRoot(Node *newRoot)
{
    this->root = newRoot;
}

void Tester::begin()
{
    // Write the head
    
    ifstream infile(testFilePath);
    string word;
    string delim(",");
    string textline;
    if(infile.good())
    {
        while(!infile.fail())
        {
            getline(infile, textline);
            split(textline, delim);
        }
    }
    infile.close();
    
    cout << "==========Test Result=========" << endl;
    int total_case = 0;
    for (int cnt = 0; cnt < NUM_CATEGORIES; cnt++) {
        cout << "[" << cnt << "] " << cateCount[cnt] << endl;
        total_case += cateCount[cnt];
    }
    cout << "[Cases count] " << total_case << endl;
}

void Tester::writeResult(int *votes)
{
    ofstream outfile(resultFilePath);
    outfile << "id,label" << "\r";
    for (int row = 0; row < NUM_TEST_ROW; row++) {
        ostringstream convert;
        convert << row+1;
        outfile << convert.str() << ",";
        
        // Count out the label according to the votes
        int maxCount = 0;
        int final_decision = 0;
        vector<int> maxVector;
        for (int cate = 0; cate < NUM_CATEGORIES; cate++)
            if (votes[row * NUM_CATEGORIES + cate] >= maxCount) {
                maxCount = votes[row * NUM_CATEGORIES + cate];
                final_decision = cate;
            }
        
        for (int cate = 0; cate < NUM_CATEGORIES; cate++)
            if (votes[row * NUM_CATEGORIES + cate] == maxCount)
                maxVector.push_back(cate);
        
        if (maxVector.size() > 1) {
            srand(unsigned(time(NULL)));
            final_decision = maxVector.at(rand() % (maxVector.size()-1));
        }
        ostringstream convert_label;
        convert_label << final_decision;
        outfile << convert_label.str() << "\r";
    }
    
    outfile.close();
}

void Tester::clearCateCount()
{
    for (int i = 0; i < NUM_CATEGORIES; i++)
        cateCount[i] = 0;
}