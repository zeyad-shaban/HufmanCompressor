#pragma once
#include "Node.h"
#include "unordered_map"
#include <iostream>
#include <list>
#include <stdio.h>
#include <string>
using namespace std;

class compress {
  private:
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>> encoder;

  public:
    // this function init a hashtable for the encoder and decoder
    // decoder is a table that contains the code and the character
    // encoder is a table that contains the character and the code ( considering root->data is the char)
    // code is initally empty as we do the recursion it will be filled with 0's or 1's
    void EncoderList(Node *root, string code) {
        if (root == NULL) {
            return;
        }
        if (root->left == NULL && root->right == NULL) {
            this->encoder[root->letters].push_back({root->letters, code});
            return;
        }
        EncoderList(root->left, code + "0");
        EncoderList(root->right, code + "1");
    }

    void saveStringToFile(const char *filename, const char *str) {
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            fputs(str, file);
            fclose(file);
        } else {
            printf("Failed to open the file.\n");
        }
    }

    string encode(std::string text) {
        string codedText = "";
        for (int i = 0; i < text.size(); i++) {
            // accessing the encoder table and getting the code of the character
            // second is the code
            codedText += this->encoder[std::string(1, text[i])].front().second;
            printf("hi");
        }
        saveStringToFile("output.com", codedText.c_str());
        return codedText;
    }
};
