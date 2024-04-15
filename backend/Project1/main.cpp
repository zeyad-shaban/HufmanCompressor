#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include "utils.h"
#include "Compress.h"
#include "app.h"
#include "httplib.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;


int main() {
	std::string userInput;
	bool useLocal;
	std::cout << "would you like to use this program \n [1]as a server \n [2]locally \n type your answer (1 or 2): ";
	std::cin >> userInput;

	if (userInput == "1") {
		useLocal = false;
	}
	else if (userInput == "2") {
		useLocal = true;
	}
	else {
		std::cout << "Invalid input. Please enter a boolean value (true/false).";
	}



	if (useLocal) {
		startAPP();
	}
	else {
		httplib::Server svr;

		svr.Options("/(.*)", [&](const httplib::Request& req, httplib::Response& res) {
			res.set_header("Access-Control-Allow-Methods", "*");
			res.set_header("Access-Control-Allow-Headers", "*");
			res.set_header("Access-Control-Allow-Origin", "*");
			res.set_header("Connection", "close");
			});

		svr.Post("/json", [](const httplib::Request& req, httplib::Response& res) {
			res.set_header("Access-Control-Allow-Origin", "*");
			// Parse the JSON from the request body
			json request_json = json::parse(req.body);

			// Extract the string from the JSON
			std::string data = request_json["message"];

			std::ofstream file("input.txt");

			if (file.is_open()) {
				file << data;
				std::cout << "read file";
				file.close(); // Close the file after writing
			}
			else {
				std::cout << "Unable to open file";
			}

			startAPP();

			std::ifstream file1("compressed_file.cod");
			std::string compressed_fileTXT;
			if (file1.is_open()) {
				std::stringstream buffer;
				buffer << file1.rdbuf();
				compressed_fileTXT = buffer.str();
				file1.close();
			}
			else {
				std::cout << "Unable to open compressed_file.cod";
			}

			std::ifstream file3("huffman_tree.json");
			std::string huffman_tree;
			if (file3.is_open()) {
				std::stringstream buffer;
				buffer << file3.rdbuf();
				huffman_tree = buffer.str();
				file3.close();
			}
			else {
				std::cout << "Unable to open compressed_file.cod";
			}

			std::ifstream file2("decoder_map.json");
			std::string decoder_mapTXT;
			if (file2.is_open()) {
				std::stringstream buffer;
				buffer << file2.rdbuf();
				decoder_mapTXT = buffer.str();
				file2.close();
			}
			else {
				std::cout << "Unable to open decoder_map.json";
			}
			json j = json::parse(decoder_mapTXT);

			// Create an array of objects
			json array = json::array();

			// Iterate over each element in the json object
			for (json::iterator it = j.begin(); it != j.end(); ++it) {
				// Create a new object and add it to the array
				json obj;
				obj["key"] = it.key();
				obj["value"] = it.value();
				array.push_back(obj);
			}
			decoder_mapTXT = array.dump(4);


			std::string response_json = R"({"compressed_file": ")" + compressed_fileTXT + R"(","decoder_map":)" + decoder_mapTXT +R"(,"huffman_tree":)"+ huffman_tree+ R"(})";
			res.set_content(response_json, "application/json");
			});


		svr.listen("localhost", 8080);

	}



	return 0;
}
