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

	const size_t chunk_size = 5 * 1024;
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
	j["c"] = node->letter;
	j["l"] = nodeToJson(node->left);
	j["r"] = nodeToJson(node->right);

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
	node->letter = j["c"].get<char>();
	node->freq = 0;
	node->left = jsonToNode(j["l"]);
	node->right = jsonToNode(j["r"]);

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
	const char sentence[] = "hello world and hello dr Nour :D";
	const size_t chunk_size = 5 * 1024;
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

void threadFileComparator(char* pathA, char* pathB, bool* done, int* state, float* progress) { // state 2 equal , state 3 inequal, 1 for file fail as usual
	const size_t BUFFER_SIZE = 5 * 1024;

	FILE* file1, * file2;
	char* buffer1, * buffer2;
	size_t bytesRead1, bytesRead2;

	file1 = fopen(pathA, "rb");
	file2 = fopen("test_decompressed.txt", "rb");

	if (file1 == NULL || file2 == NULL) {
		printf("Error opening file.\n");
		*state = 1;
		*done = true;
		return;
	}

	buffer1 = (char*)malloc(BUFFER_SIZE);
	buffer2 = (char*)malloc(BUFFER_SIZE);

	if (buffer1 == NULL || buffer2 == NULL) {
		*state = -1;
		*done = true;

		return;
	}

	unsigned long totSize = 0;
	double totRead = 0;
	fseek(file1, 0, SEEK_END);
	totSize += ftell(file1);
	fseek(file1, 0, SEEK_SET);

	fseek(file2, 0, SEEK_END);
	totSize += ftell(file2);
	fseek(file2, 0, SEEK_SET);


	do {
		bytesRead1 = fread(buffer1, 1, BUFFER_SIZE, file1);
		bytesRead2 = fread(buffer2, 1, BUFFER_SIZE, file2);
		totRead += bytesRead1 + bytesRead2;
		*progress = totRead / totSize;

			if (bytesRead1 != bytesRead2 || memcmp(buffer1, buffer2, bytesRead1) != 0) {
				*state = 3; // hmm i could be evil and always return 2 the success state
				*done = true;
				free(buffer1);
				free(buffer2);
				fclose(file1);
				fclose(file2);
				return;
			}
	} while (bytesRead1 == BUFFER_SIZE);


	free(buffer1);
	free(buffer2);
	fclose(file1);
	fclose(file2);

	*state = 2;
	*done = true;
	return;
}


void OpenFileDialog(const wchar_t* filter, char* editMe) {
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


	if (!GetOpenFileNameW(&ofn)) return;

	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, editMe, wcslen(filename) + 1, filename, _TRUNCATE);

	for (size_t i = 0; editMe[i] != '\0'; i++) {
		if (editMe[i] == '\\') {
			editMe[i] = '/';
		}
	}
}

void OpenDirectoryDialog(char* editMe) {
	static wchar_t path[MAX_PATH];

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select Directory";
	LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);

	if (!pidl) return;


	SHGetPathFromIDListW(pidl, path);

	IMalloc* imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc))) {
		imalloc->Free(pidl);
		imalloc->Release();
	}

	if (path != NULL) {
		size_t convertedChars = 0;
		wcstombs_s(&convertedChars, editMe, wcslen(path) + 1, path, _TRUNCATE);

		for (size_t i = 0; editMe[i] != '\0'; i++) {
			if (editMe[i] == '\\') {
				editMe[i] = '/';
			}
		}
	}
}