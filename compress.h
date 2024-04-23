#pragma once
#include "Node.h"
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <fstream>
#include "utils.h"
using namespace std;

class compress {
    map<std::string, std::string> encoder;
    map<std::string, std::string> decoder;
public:
	void createMaps(Node* root, string code) {
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
    string compressing(string filePath, string filename) {
        string codedText = "";
		std::fstream file(filePath);
		if (!file.is_open()) return "";

        char ch;
		while (file.get(ch))
            if (ch >= 0 && ch < 128) codedText.append(encoder[string(1, ch)]) ;

		saveStringToFile((string("./data/") + filename + string(".com")).c_str(), codedText.c_str());
        saveDecoder(filename + "_decoder");
        saveEncoder(filename + "_encoder");
        return codedText;
    }
    string decompressing(const string text, const string filename) {
        string decodedText;
        string code;
        int c = 0;
        decodedText.reserve(text.size());

        while (c < text.size()) {
            code += text[c];
            auto it = decoder.find(code);
            if (it != decoder.end()) {
                decodedText.append(it->second);
                code.clear();
            }
            c++; 
        }

        // Save decodedText to file
        saveStringToFile((string("./data/decompressed_") + filename + string(".txt")).c_str(), decodedText.c_str());

        return decodedText;
    }

private:
    void saveEncoder(string filename) {
        string data = "Keys | Values\n";
        for (auto it = encoder.begin(); it != encoder.end(); it++) {
            data += it->first + " | " + it->second + "\n";
        }
        saveStringToFile((string("./data/") + filename + string(".txt")).c_str(), data.c_str());
    }
    void saveDecoder(string filename) {
        string data = "Keys | Values\n";
        for (auto it = decoder.begin(); it != decoder.end(); it++) {
            data += it->first + " | " + it->second + "\n";
        }
        saveStringToFile((string("./data/") + filename + string(".txt")).c_str(), data.c_str());
    }
        void printEncoder() {
        for (auto it = encoder.begin(); it != encoder.end(); it++) {
            cout << it->first << " " << it->second << endl;
        }
    }
};
