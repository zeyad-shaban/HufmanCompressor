#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
//#include "utils.h"
//#include "LocalApp.h"
//#include "StartServer.h"
#include "RayMainFrame.h"

//using json = nlohmann::json;

using namespace std;


int main(int argc, char** argv) {
	//bool devMode = false;
	//if (devMode) {
	//	std::string dirpath = "C:/Users/zeyad/OneDrive/Desktop/test/scripts"; // please do not delete it instead create a new variable to override it if needed 
	//	std::string filename = "10gbInput";
	//	int compressionOrder = 100;

	//	startCompressing(dirpath + "/" + filename + ".txt", dirpath, compressionOrder);
	//	StartDecompressing(dirpath + "/" + filename + ".bin", dirpath + "/" + filename + "_tree.json", dirpath);
	//	return 0;
	//}

	//std::string userInput;

	//std::cout << "would you like to use this program \n [1]as a server (Not recommended for larger files) \n [2]\n Choice: ";
	//// std::cin >> userInput2;
	//userInput = "3";

	//if (userInput == "1")
	//	StartServer();
	//else if (userInput == "2") {
	//	wxEntry(argc, argv);
	//}
	//else if (userInput == "3")
		RayMainFrame();
	//else
		//std::cout << "Invalid input";
}



/*
TODO

Loading bar

Fix win GUI & add in for max order

err handle for missing files and handle if tmp0 or tmp1 is already there and chnage the handle for tmp until we find something that would work

Ability to generate file

Take max compression order
*/