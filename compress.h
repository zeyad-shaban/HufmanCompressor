#pragma once
#include <iostream>
#include <list>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Node.h"
using namespace std;

class compress {
    list<pair<char, string>> encoder[128];

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

  private:
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
            codedText += this->encoder[text[i]].front().second;
        }
        saveStringToFile("output.com", codedText.c_str());
        return codedText;
    }
};
