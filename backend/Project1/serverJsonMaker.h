#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"
#include <bitset>
#include <fstream>
#include <vector>
using namespace std;
using json = nlohmann::json;

string readBinaryFileAsBits(const string& filename) {
    // Open the binary file
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Unable to open " << filename << endl;
        return "";
    }

    // Read the file into a vector of bytes
    vector<unsigned char> bytes((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Convert each byte to bits using std::bitset
    string bits;
    for (unsigned char byte : bytes) {
        bits += bitset<8>(byte).to_string();
    }

    return bits;
}

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open " << filename << endl;
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}
json readFileJson(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open " << filename << endl;
        return json::array();
    }
    json j;
    file >> j;
    file.close();
    return j;
}
string getCompressedFiles() {
    string compressed_fileTXT = readBinaryFileAsBits("serverData/input_compressed.bin");
    string frequency_tableTXT = readFile("serverData/frequency_table.txt");
    json huffman_tree = readFileJson("./serverData/input_tree.json");
    json decoder_map = readFileJson("./serverData/input_decoder.json");

    json response_json = {
        {"compressed_file", compressed_fileTXT},
        {"decoder_map", decoder_map},
        {"huffman_tree", huffman_tree},
        {"frequency_table", frequency_tableTXT}
    };

    //cout << response_json.dump(4);
    return response_json.dump(4);
}
