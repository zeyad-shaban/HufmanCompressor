#pragma once
 #include <iostream>
#include "Node.h"


int startCompressing(std::string filePath, std::string dirPath, int maxOrder = 1, Node** rootPtr = nullptr);