#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include "utils.h"
#include "Compressor.h"
#include <thread>
#include <future>

int startCompressing(string filePath, string dirPath, int maxOrder = 1, Node** rootPtr = nullptr);