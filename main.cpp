//
//  main.cpp
//  binary_search_tree
//
//  Created by ¡÷ ≥ı—Ù on 12-12-13.
//  Copyright (c) 2012ƒÍ __MyCompanyName__. All rights reserved.
//

#include "FileReader.h"
#include "DecisionTree.h"
#include <iostream>
#include <string>
using namespace std;


int main(int argc, const char * argv[])
{
    FileReader reader;
    reader.readFileList("/Users/apple/Developer/Random-Forest/Random-Forest/", -1);

//    DecisionTree tree(reader.dataSet);
//    tree.createTree();
//    Node *root = tree.getRoot();
    
    return 0;
}
