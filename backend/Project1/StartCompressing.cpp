#include "StartCompressing.h"
#include "utils.h"
 #include "Compressor.h"
#include <string>

int startCompressing(std::string filePath, std::string dirPath, int maxOrder, Node** rootPtr) {
	std::string TMP_KEY = "tmp";

	std::string base_filename = filePath.substr(filePath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);

	Node** treeArr = (Node**)malloc(sizeof(Node) * maxOrder);
	int treeArrI = 0;

	if (!treeArr) {
		std::cout << "Failed to allocate memroy for your order, please try smaller number\n";
		return -1;
	}


	int tmpTurn = 1;
	long long minSize = 9223372036854775807;
	long long thisSize;
	int order = 0;
	for (; order < maxOrder; order++) {
		std::string tmpIn = order == 0 ? filePath : dirPath + "/" + TMP_KEY + std::to_string(tmpTurn);
		tmpTurn = tmpTurn == 1 ? 0 : 1;

		std::string tmpOut = dirPath + "/" + (order == maxOrder - 1 ? file_without_extension + ".bin" : TMP_KEY + std::to_string(tmpTurn));

		// Generate frequency table
		int freqTable[256] = { 0 };
		std::cout << "->Generating frequency-table...\n";
		time_t start, end;
		time(&start);
		if (genFreqTable(tmpIn, freqTable)) {
			time(&end);
			std::cout << "FreqTable Generated In: " << end - start << "sec" << std::endl;
		}
		else
			std::cout << "FreqTable faild:can't open file\n";


		// Create min heap for Huffman tree
		MinHeap* heap = new MinHeap(256);
		bool overflow = false;

		for (int i = 0; i < 256; i++)
			if (freqTable[i])
				heap->insertValues(i, freqTable[i]);

		// Generate Huffman tree
		treeArr[treeArrI] = tregen(heap);
		delete heap;


		// Compress the file
		Compressor compressor = Compressor(); // WHY IS IT EVEN IN A CLASS JUST PUT IT IN A VOID FUNCTION LIEK ALL NORMAL PEOPLE DO
		bool validPath = true;
		std::cout << "->compressing using HuffmanTree...\n";

		time(&start);
		thisSize = compressor.compressing(treeArr[treeArrI], tmpIn, tmpOut);
		time(&end);
		std::cout << "Done copmressing in " << end - start << "sec" << std::endl;

		treeArr[treeArrI] = treeArr[treeArrI];
		treeArrI++;


		if (minSize - thisSize > 1000 && thisSize > 1000) minSize = thisSize;
		else {
			++order;
			rename(tmpOut.c_str(), (dirPath + "/" + file_without_extension + ".bin").c_str());
			break;
		}
	}

	writeTreeArrToJsonFile(treeArr, order, dirPath + "/" + file_without_extension + "_tree.json");

	for (int i = 0; i < order; ++i)
		delete treeArr[i];
	free(treeArr);

	remove((dirPath + "/" + TMP_KEY + "0").c_str());
	remove((dirPath + "/" + TMP_KEY + "1").c_str());

	return 0;
}