#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "utils.h"
#include "StartCompressing.h"
#include "StartDecompressing.h"
#include "LocalApp.h"
#include "StartServer.h"

using json = nlohmann::json;

using namespace std;


int main(int argc, char** argv) {
	std::string userInput2;
	bool useLocal = false;
	bool useCompress = false;



	std::cout << "would you like to use this program \n [1]as a server (Not recommended for larger files) \n [2]locally (looks ugly..) \n type your answer (1 or 2): ";
	std::cin >> userInput2;

	if (userInput2 == "1")
		useLocal = false;
	else if (userInput2 == "2")
		useLocal = true;
	else
		std::cout << "Invalid input";

	if (useLocal)
		wxEntry(argc, argv);
	else
		StartServer();
}