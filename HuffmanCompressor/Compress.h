#pragma once
#include "Node.h"
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <fstream>
using namespace std;

class compress {
	// TODO SAVE ENCODER AND DECODER AND JSON AND LOAD LATER
	map<std::string, std::string> encoder;
	map<std::string, std::string> decoder;

public:
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

	string compressing(string filePath, bool* validPath) {
		string codedText = "";
		std::fstream file(filePath);
		if (!file.is_open()) {
			*validPath = false;
			return "";
		}

		char ch;
		while (file.get(ch))
			if (ch >= 0 && ch < 128) codedText += encoder[string(1, ch)];

		file.close();
		return codedText;
	}
	string decompressing(string text, string filename) {
		string decodedText = "";
		string code = "";
		for (int i = 0; i < text.size(); i++) {
			code += text[i];
			if (decoder.find(code) != decoder.end()) {
				decodedText += decoder[code];
				code = "";
			}
		}
		//saveStringToFile((string("./data/decompressed_") + filename + string(".txt")).c_str(), decodedText.c_str());
		return decodedText;
	}

	// printing the encoder table
	void printEncoder() {
		for (auto it = encoder.begin(); it != encoder.end(); it++) {
			cout << it->first << " " << it->second << endl;
		}
	}
};
