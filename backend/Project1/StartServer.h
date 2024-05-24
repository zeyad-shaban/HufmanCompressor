// keepting this for the effort :'(

#pragma once
#include <iostream>
#include "httplib.h"
#include "nlohmann/json.hpp"
#include "StartCompressing.h"
#include "StartDecompressing.h"
#include <thread>
#include <future>
#include <fstream>
#include "serverJsonMaker.h"

using json = nlohmann::json;

void StartServer() {
	_mkdir("./serverData");
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

		std::ofstream file("./serverData/input.txt");

		if (!file.is_open()) {
			std::cout << "READING-FAILED:Unable to open file\n";
			res.set_content("Error: Unable to open file", "text/plain");
			return;
		}

		file << data;
		std::cout << "READING-COMPLETED:input data saved to 'input.txt'\n\n";
		file.close();

		std::promise<void> compressionPromise;
		std::future<void> compressionFuture = compressionPromise.get_future();

		std::cout << "->compressing operation started...\n";

		std::thread compressionThread([&]() {
			startCompressing("./serverData/input.txt", "./serverData");
			std:cout << "DONE WITH COMPRESSIONNN\n";
		compressionPromise.set_value(); // Signal that the task is done
		});
		compressionFuture.wait();


		compressionThread.join();


		res.set_content(getCommpressedFiles(), "application/json");

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

		std::ofstream file1("./serverData/input_compressed.bin");

		if (file1.is_open()) {
			file1 << compressedFileValue;
			std::cout << "READING-COMPLETED:compressed data saved to 'input_compressed_file.com'\n\n";
			file1.close(); // Close the file after writing
		}
		else {
			std::cout << "READING-FAILED:Unable to open file\n";
		}
		std::ofstream file2("./serverData/input_decoder.json");

		if (file2.is_open()) {
			file2 << decoderFileValue;
			std::cout << "READING-COMPLETED:decodermap data saved to 'decoder_map.json'\n\n";
			file2.close(); // Close the file after writing
		}
		else {
			std::cout << "READING-FAILED:Unable to open file\n";
		}


		std::promise<void> DecompressionPromise;
		std::future<void> DecompressionFuture = DecompressionPromise.get_future();

		std::cout << "->De-compressing operation started...\n";

		std::thread DecompressionThread([&]() {
		StartDecompressing("./serverData/input_compressed.bin", "./serverData/input_decoder.json", "./serverData");
		std:cout << "DONE WITH DE-COMPRESSIONNN\n";
			DecompressionPromise.set_value(); // Signal that the task is done
			});
		DecompressionFuture.wait();


		DecompressionThread.join();



		std::ifstream file3("./serverData/input_compressed_decompressed.txt");
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