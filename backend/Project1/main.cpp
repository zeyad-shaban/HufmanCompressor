#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "utils.h"
#include "LocalApp.h"
#include "StartServer.h"

using json = nlohmann::json;

using namespace std;


int main(int argc, char** argv) {
	bool debugMode = true;
	if (debugMode) {
		std::string dirpath = "C:/Users/zeyad/OneDrive/Desktop/test";
		//startCompressing(dirpath + "/hello.txt", dirpath, 2);
		 StartDecompressing(dirpath + "/fibonacci_compressed.bin", dirpath + "/fibonacci.json", dirpath);
		return 0;
	}

	std::string userInput2;

	std::cout << "would you like to use this program \n [1]as a server (Not recommended for larger files) \n [2]locally (looks ugly..) \n type your answer (1 or 2): ";
	std::cin >> userInput2;

	if (userInput2 == "1")
		StartServer();
	else if (userInput2 == "2")
		wxEntry(argc, argv);
	else
		std::cout << "Invalid input";
}