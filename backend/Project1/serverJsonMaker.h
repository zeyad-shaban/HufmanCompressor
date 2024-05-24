#include <future>
#include <thread>
#include <iostream>
#include "httplib.h"
#include "nlohmann/json.hpp"
#include "StartCompressing.h"
#include "StartDecompressing.h"
using namespace std;
using json = nlohmann::json;

string getCommpressedFiles() {
	std::ifstream file1("serverData/input_compressed.bin");
	std::string compressed_fileTXT;
	if (file1.is_open()) {
		std::stringstream buffer;
		buffer << file1.rdbuf(); 
		compressed_fileTXT = buffer.str();
		file1.close();
	}
	else {
		std::cout << "Unable to open input_compressed.bin";
	}
	std::ifstream file2("serverData/frequency_table.txt");
	std::string frequency_tableTXT;
	if (file2.is_open()) {
		std::stringstream buffer;
		buffer << file2.rdbuf();
		frequency_tableTXT = buffer.str();
		file2.close();
	}
	else {
		std::cout << "Unable to open compressed_file.cod";
	}


	std::ifstream file3("./serverData/input_tree.json");
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

	std::ifstream file4("./serverData/input_decoder.json");
	std::string decoder_mapTXT;
	if (file4.is_open()) {
		std::stringstream buffer;
		buffer << file4.rdbuf();
		decoder_mapTXT = buffer.str();
		file4.close();
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
	//decoder_mapTXT = array.dump(4);


	std::string response_json = R"({"compressed_file": ")" + compressed_fileTXT + R"(","decoder_map":)" + decoder_mapTXT + R"(,"huffman_tree":)" + huffman_tree + R"(,"frequency_table": ")" + frequency_tableTXT + R"("})";
	return response_json;

}
