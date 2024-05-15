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
		std::string filename = "small";
		startCompressing(dirpath + "/" + filename + ".txt", dirpath, 2);
		 StartDecompressing(dirpath + "/" + filename + "_compressed.bin", dirpath + "/" + filename + "_tree.json", dirpath);
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