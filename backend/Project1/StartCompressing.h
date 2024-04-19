#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include "utils.h"
#include "Compress.h"

int startCompressing(string filePath, string dirPath, unordered_map<string, string>* encoderPtr=nullptr, Node** rootPtr=nullptr, string* textPrevPtr=nullptr);