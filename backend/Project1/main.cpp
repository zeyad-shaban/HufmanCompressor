#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "utils.h"
#include "LocalApp.h"
#include "StartServer.h"

using json = nlohmann::json;

using namespace std;


int main(int argc, char** argv) {
	std::string userInput2;
	
	std::cout << "would you like to use this program \n [1]as a server (Not recommended for larger files) \n [2]locally (looks ugly..) \n type your answer (1 or 2): ";
	std::cin >> userInput2;
	
	if (userInput2 == "1")
		StartServer();
	else if (userInput2 == "2")
		wxEntry(argc, argv);
	else
		std::cout << "Invalid input";

	startCompressing("C:/Users/zeyad/OneDrive/Desktop/test/1/helloSmall_compressed.bin", "C:/Users/zeyad/OneDrive/Desktop/test/1/2");
	StartDecompressing("C:/Users/zeyad/OneDrive/Desktop/test/f_compressed_compressed.bin", "C:/Users/zeyad/OneDrive/Desktop/test/f_compressed_tree.json", "C:/Users/zeyad/OneDrive/Desktop/test");
}