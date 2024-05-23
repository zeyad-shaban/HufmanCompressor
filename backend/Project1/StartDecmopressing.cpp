#include "StartDecompressing.h"
#include <string>
#include "Compressor.h"
#include "utils.h"

void StartDecompressing(std::string compressedFilePath, std::string treePath, std::string dirPath, bool* done, int* state, float* progress) {
	std::string TMP_KEY = "tmp";

	std::string base_filename = compressedFilePath.substr(compressedFilePath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);

	Compressor* compressor = new Compressor();

	int size = 0;
	Node** treeArr = readTreeArrFromJsonFile(treePath, &size);

	int tmpTurn = 1;
	for (int i = size - 1; i >= 0; --i) {
		std::string tmpIn = i == size - 1 ? compressedFilePath : dirPath + "/" + TMP_KEY + std::to_string(tmpTurn);
		tmpTurn = tmpTurn == 1 ? 0 : 1;

		std::string tmpOut = dirPath + "/" + (i == 0 ? file_without_extension + "_decompressed.txt" : TMP_KEY + std::to_string(tmpTurn));

		compressor->decompressing(treeArr[i], tmpIn, tmpOut);
	}

	remove((dirPath + "/" + TMP_KEY + "0").c_str());
	remove((dirPath + "/" + TMP_KEY + "1").c_str());

	*state = 0;
	*done = true;
	return;
}