#include <iostream>
#include <nlohmann/json.hpp>
#include "utils.h"
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include "direct.h"
#include <fstream>


 using json = nlohmann::json;


Node* tregen(MinHeap* heap) {
	while (heap->heapPtr > 1) {
		Node* node1 = heap->extractMin();
		Node* node2 = heap->extractMin();

		int hPtr = heap->heapPtr;

		heap->arr[hPtr].letter = NULL;
		heap->arr[hPtr].freq = node1->freq + node2->freq;
		heap->arr[hPtr].left = node1;
		heap->arr[hPtr].right = node2;

		heap->orderInsertedNode();
	}
	return heap->extractMin();
}

bool genFreqTable(std::string filePath, int* freqTable) {
	FILE* file; bool fileErr = fopen_s(&file, filePath.c_str(), "rb");
	if (fileErr) return false;

	const size_t chunk_size = 1024 * 1024;
	unsigned char buffer[chunk_size];
	int charsRead = 0;
	while ((charsRead = fread(buffer, 1, chunk_size, file)) > 0)
		for (int i = 0; i < charsRead; i++)
			freqTable[buffer[i]]++;

	fclose(file);
	return true;
}
bool saveStringToFile(std::string path, std::string str) {
	std::ofstream file(path);
	if (!file) return false;

	file << str;

	return true;
}

json nodeToJson(Node* node) {
	if (node == nullptr)return nullptr;

	json j;
	j["letter"] = node->letter;
	j["freq"] = node->freq;
	j["left"] = nodeToJson(node->left);
	j["right"] = nodeToJson(node->right);

	return j;
}

void writeTreeArrToJsonFile(Node* treeArr[], int treeSize, const std::string& filename) {
	json jsonArr = json::array();

	for (int i = 0; i < treeSize; ++i) jsonArr.push_back(nodeToJson(treeArr[i]));

	std::ofstream file(filename);
	file << jsonArr.dump();
	file.close();
}


Node* jsonToNode(const json& j) {
	if (j.is_null()) return nullptr;

	Node* node = new Node;
	node->letter = j["letter"].get<char>();
	node->freq = j["freq"].get<int>();
	node->left = jsonToNode(j["left"]);
	node->right = jsonToNode(j["right"]);

	return node;
}

Node** readTreeArrFromJsonFile(const std::string& filename, int* size) {
	std::ifstream file(filename);
	if (!file.is_open()) return nullptr;

	json jsonArr;
	file >> jsonArr;
	file.close();

	*size = jsonArr.size();
	Node** treeArr = (Node**)malloc(*size * sizeof(Node));

	int i = 0;
	for (const auto& jsonObj : jsonArr) {
		treeArr[i] = jsonToNode(jsonObj);
		i++;
	}

	return treeArr;
}

void runExecutable(const char* filePath) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, filePath, -1, NULL, 0);
	wchar_t* wpath = new wchar_t[size_needed];
	MultiByteToWideChar(CP_UTF8, 0, filePath, -1, wpath, size_needed);

	char currentDirectory[MAX_PATH];
	_getcwd(currentDirectory, sizeof(currentDirectory));

	std::string targetDir;
	const size_t last_slash_idx = std::string(filePath).rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		targetDir = std::string(filePath).substr(0, last_slash_idx);
	}

	_chdir(targetDir.c_str());

		std::string base_filename = std::string(filePath).substr(std::string(filePath).find_last_of("/\\") + 1);

	system(("\"" + base_filename + "\"").c_str());

	_chdir(currentDirectory);

	delete[] wpath;
}

void threadFileGenerator(bool* done, int* state, float* progress, float generatedVal, char* filename) {
	const char sentence[] = "hello Dr Nour";
	const size_t chunk_size = 1024 * 1024;
	const long long gb1 = 1024LL * 1024LL * 1024LL * 1LL;

	FILE* file = fopen(filename, "wb");

	if (!file) {
		*state = 1;
		*done = true;
		return;
	}

	long long written_bytes = 0;
	double dblTot = gb1 * generatedVal;

	while (file && written_bytes < dblTot) {
		*progress = written_bytes / dblTot;
		size_t write_size = strlen(sentence) < chunk_size ? strlen(sentence) : chunk_size;
		fwrite(sentence, 1, write_size, file);
		written_bytes += write_size;
	}

	fclose(file);
	*state = 0;
	*done = true;
	return;
}


const wchar_t* OpenFileDialog(const wchar_t* filter) {
	static wchar_t filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = wcschr(filter, L'\0') + 1;

	if (GetOpenFileNameW(&ofn)) {
		return filename;
	}
	else {
		return NULL;
	}
}

const wchar_t* OpenDirectoryDialog() {
	static wchar_t path[MAX_PATH];

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select Directory";
	LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);

	if (pidl != 0) {
		SHGetPathFromIDListW(pidl, path);

		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc))) {
			imalloc->Free(pidl);
			imalloc->Release();
		}
		return path;
	}
	return NULL;
}
