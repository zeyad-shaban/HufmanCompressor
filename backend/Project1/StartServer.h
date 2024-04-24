#pragma once
#include <iostream>
#include "httplib.h"
#include "nlohmann/json.hpp"
#include "StartCompressing.h"
#include "StartDecompressing.h"

using json = nlohmann::json;

void StartServer(){
	httplib::Server svr;

	svr.Options("/(.*)", [&](const httplib::Request& req, httplib::Response& res) {
		res.set_header("Access-Control-Allow-Methods", "*");
		res.set_header("Access-Control-Allow-Headers", "*");
		res.set_header("Access-Control-Allow-Origin", "*");
		res.set_header("Connection", "close");
		});

	svr.Post("/compress", [](const httplib::Request& req, httplib::Response& res) {
		res.set_header("Access-Control-Allow-Origin", "*");
		// Parse the JSON from the request body
		json request_json = json::parse(req.body);

		std::cout << "\n----INCOMING REQUEST CONTENTS:" << std::endl;
		std::cout << "-Method: " << req.method << std::endl;
		std::cout << "-Path: " << req.path << std::endl;
		std::cout << "-Origin: " << req.get_header_value("Origin") << std::endl;
		std::cout << "-User-Agent: " << req.get_header_value("User-Agent") << "\n\n" << std::endl;

		// Extract the string from the JSON
		std::string data = request_json["message"];

		std::ofstream file("input.txt");

		if (file.is_open()) {
			file << data;
			std::cout << "READING-COMPLETED:input data saved to 'input.txt'\n\n";
			file.close(); // Close the file after writing
		}
		else {
			std::cout << "READING-FAILED:Unable to open file\n";
		}

		std::cout << "->compressing operation started...\n";
		startCompressing("filePath", "dirPath");

		std::ifstream file1("compressed_file.com");
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
		std::ifstream file2("frequency_table.txt");
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

		std::ifstream file4("decoder_map.json");
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
		res.set_content(response_json, "application/json");
		});


	svr.Post("/decompress", [](const httplib::Request& req, httplib::Response& res) {
		res.set_header("Access-Control-Allow-Origin", "*");
		// Parse the JSON from the request body
		json request_json = json::parse(req.body);

		std::cout << "\n----INCOMING REQUEST CONTENTS:" << std::endl;
		std::cout << "-Method: " << req.method << std::endl;
		std::cout << "-Path: " << req.path << std::endl;
		std::cout << "-Origin: " << req.get_header_value("Origin") << std::endl;
		std::cout << "-User-Agent: " << req.get_header_value("User-Agent") << "\n\n" << std::endl;

		std::string compressedFileValue = request_json.at("compressedFile").get<std::string>();
		std::string decoderFileValue = request_json.at("decoderFile").get<std::string>();

		std::ofstream file1("compressed_file.com");

		if (file1.is_open()) {
			file1 << compressedFileValue;
			std::cout << "READING-COMPLETED:compressed data saved to 'compressed_file.com'\n\n";
			file1.close(); // Close the file after writing
		}
		else {
			std::cout << "READING-FAILED:Unable to open file\n";
		}
		std::ofstream file2("decoder_map.json");

		if (file2.is_open()) {
			file2 << decoderFileValue;
			std::cout << "READING-COMPLETED:decodermap data saved to 'decoder_map.json'\n\n";
			file2.close(); // Close the file after writing
		}
		else {
			std::cout << "READING-FAILED:Unable to open file\n";
		}


		//StartDecompressing();

		std::ifstream file3("decompressed.txt");
		std::string decompressedTXT;
		if (file3.is_open()) {
			std::stringstream buffer;
			buffer << file3.rdbuf();
			decompressedTXT = buffer.str();
			file3.close();
		}
		else {
			std::cout << "Unable to open compressed_file.cod";
		}

		std::string response_json = R"({"decompressed": ")" + decompressedTXT + R"("})";

		res.set_content(response_json, "application/json");
		});


	svr.listen("localhost", 8080);
}