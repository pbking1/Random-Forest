//
//  FileWriter.h
//  Random-Forest
//
//  Created by Cho-Yeung Lam on 17/6/14.
//  Copyright (c) 2014 Cho-Yeung Lam. All rights reserved.
//

#ifndef __Random_Forest__FileWriter__
#define __Random_Forest__FileWriter__

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Constants.h"
#include <vector>
using namespace std;

class FileWriter
{  
public:
    FileWriter();
    void writeResult(int *votes, string resultFilePath);
};

#endif /* defined(__Random_Forest__FileWriter__) */