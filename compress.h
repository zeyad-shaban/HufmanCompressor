#include "Node.h"
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
using namespace std;

class compress {

    map<std::string, std::string> encoder;
    map<std::string, std::string> decoder;

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

  public:
    void createMaps(Node *root, string code) {
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

    string compressing(char *filePath) {
        string codedText = "";
        FILE *file = fopen(filePath, "r");
        if (!file) return "";

        char ch;
        do {
            ch = fgetc(file);
            if (ch >= 0 && ch < 128) codedText += encoder[string(1, ch)];
        } while (ch != EOF);

        saveStringToFile("./data/code.cod", codedText.c_str());
        return codedText;
    }
    string decompressing(string text) {
        string decodedText = "";
        string code = "";
        for (int i = 0; i < text.size(); i++) {
            code += text[i];
            if (decoder.find(code) != decoder.end()) {
                decodedText += decoder[code];
                code = "";
            }
        }
        saveStringToFile("./data/original.txt", decodedText.c_str());
        return decodedText;
    }

    // printing the encoder table
    void printEncoder() {
        for (auto it = encoder.begin(); it != encoder.end(); it++) {
            cout << it->first << " " << it->second << endl;
        }
    }
};
