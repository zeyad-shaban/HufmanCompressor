#pragma once
#include "Node.h"
#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <fstream>
using namespace std;

class compress {
private:
	int bufferSize = 4096;

public:
	unordered_map<std::string, std::string> encoder;
	unordered_map<std::string, std::string> decoder;

	compress() {
		encoder = unordered_map < std::string, std::string>();
		decoder = unordered_map < std::string, std::string>();
	}

	void createMaps(Node* root, string code = "") {
		if (root == NULL) {
			return;
		}
		if (root->left == NULL && root->right == NULL) {
			encoder[root->letters] = code;
			decoder[code] = root->letters;
			return;
		}
		createMaps(root->left, code + "0");
		createMaps(root->right, code + "1");
	}

	string compressing(string filePath, string outputFilePath, bool* validPath, int prevSize = 300) {
		ifstream file(filePath);
		ofstream outputFile(outputFilePath);
		if (!file.is_open() || !outputFile.is_open()) {
			*validPath = false;
			return "";
		}

		string codedTextPrev = "";
		string buffer = "";

		char ch;
		while (file.get(ch)) {
			if (!(ch >= 0 && ch < 128)) continue;

			if (codedTextPrev.size() <= prevSize) codedTextPrev += encoder[string(1, ch)];

			buffer += encoder[string(1, ch)];
			if (buffer.size() >= bufferSize) {
				outputFile << buffer;
				buffer = "";
			}
		}
		outputFile << buffer;

		if (codedTextPrev.size() >= prevSize) codedTextPrev += "...";
		return codedTextPrev;
	}
	string decompressing(string compressedFilePath, string outputFilePath, bool* validPath, int prevSize = 300) {
		ifstream compressedFile(compressedFilePath);
		ofstream outputFile(outputFilePath);

		if (!compressedFile.is_open() || !outputFile.is_open()) {
			*validPath = false;
			return "";
		}


		string decodedTextPrev = "";
		string code = "";
		string buffer = "";

		char ch;
		while (compressedFile.get(ch)) {
			code += ch;
			if (decoder.find(code) != decoder.end()) {
				if (decodedTextPrev.size() <= prevSize) decodedTextPrev += decoder[code];

				buffer += decoder[code];
				code = "";

				if (buffer.size() >= bufferSize) {
					outputFile << buffer;
					buffer = "";
				}

			}
		}
		outputFile << buffer;


		if (decodedTextPrev.size() >= prevSize) decodedTextPrev += "...";

		return decodedTextPrev;
	}

	// printing the encoder table
	void printEncoder() {
		for (auto it = encoder.begin(); it != encoder.end(); it++) {
			cout << it->first << " " << it->second << endl;
		}
	}
};