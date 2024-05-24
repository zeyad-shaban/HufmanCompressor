#include <direct.h>
#include "raylib.h"
#include "StartCompressing.h"
#include "StartDecompressing.h"
#include "utils.h"
#include <thread>
#include <future>
#include "filedialogs.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


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


void panelViewer(bool* popupActive, int* state, bool* showGames, bool* showServer, char* generatedFileName, const char** filterTextPatterns, float* generatedVal, bool* orderInputActive, Rectangle gamePanel, float winWidth, Texture2D animVsWitherImg, Texture2D geoSmashImg, Texture2D riskOfImg) {
	if (*showGames) {
		GuiPanel(gamePanel, "Play something while waiting");

		if (GuiButton(Rectangle{ winWidth - 395, 40, 190, 190 }, ""))
			runExecutable(".\\games\\AnimationVsWither\\main.exe");
		DrawTextureRec(animVsWitherImg, Rectangle{ 0, 0, 190, 190 }, Vector2{ winWidth - 395, 40 }, WHITE);

		if (GuiButton(Rectangle{ winWidth - 200, 40, 200, 200 }, ""))
			runExecutable(".\\games\\GeoSmash\\main.exe");
		DrawTextureRec(geoSmashImg, Rectangle{ 0, 0, 200, 200 }, Vector2{ winWidth - 200, 40 }, WHITE);

		if (GuiButton(Rectangle{ winWidth - 395, 200, 200, 200 }, ""))
			runExecutable(".\\games\\RiskOfGettingTrailblaze\\Risk of getting Railed.exe");
		DrawTextureRec(riskOfImg, Rectangle{ 0, 0, 200, 200 }, Vector2{ winWidth - 395, 200 }, WHITE);
	}
	GuiToggle(Rectangle{ winWidth - 40, 0, 40, 40 }, *showGames ? "#128#" : "#152#", showGames);

	if (*showServer) {
		*orderInputActive = false;
		GuiPanel(gamePanel, "Tools");

		GuiLabel(Rectangle{ winWidth - 395, 40, winWidth, 100 }, "#8# File Filler");
		GuiLabel(Rectangle{ winWidth - 375, 80, winWidth, 100 }, "#217# Path: ");
		if (GuiTextBox(Rectangle{ winWidth - 230, 115, 150, 40 }, generatedFileName, sizeof(generatedFileName), false)
			|| GuiButton(Rectangle{ winWidth - 230 + 150, 115, 40, 40 }, "#5#")) {
			const char* selectedTextFile = selectFileDialog("Select a file", NULL, 1, filterTextPatterns, "Text Files");

			if (selectedTextFile != NULL) {
				strncpy(generatedFileName, selectedTextFile, strlen(selectedTextFile));
				generatedFileName[strlen(selectedTextFile)] = '\0';
				for (int i = 0; generatedFileName[i] != '\0'; i++)
					if (generatedFileName[i] == '\\')
						generatedFileName[i] = '/';
			}
		}

		char label[50];
		sprintf(label, "#218# Size in GB: %.2f", *generatedVal);
		GuiLabel(Rectangle{ winWidth - 375, 130, winWidth, 100 }, label);

		GuiSliderBar(Rectangle{ winWidth - 375, 200, winWidth, 20 }, "", NULL, generatedVal, 0.0f, 100.0f);

		if (GuiButton(Rectangle{ winWidth - 350, 250, 300, 40 }, "Fill")) {
			bool done = false;
			float progress = 0;


			std::thread worker(threadFileGenerator, &done, state, &progress, *generatedVal, generatedFileName);
			worker.detach();

			while (!done) {
				BeginDrawing();
				ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
				GuiProgressBar(Rectangle{ winWidth / 2 - 200, 720 / 2 - 20, 400, 40 }, NULL, NULL, &progress, 0, 1);
				EndDrawing();
			}

			*popupActive = true;
		}


	}
	GuiToggle(Rectangle{ winWidth - 80, 0, 40, 40 }, *showServer ? "#128#" : "#198#", showServer);
}



int WinMain() {
	const int fontSize = 30;
	const float winWidth = 1280;
	const float winHeight = 720;
	InitWindow(winWidth, winHeight, "3A-Z");

	Texture2D animVsWitherImg = LoadTexture("./assets/AnimationVsWither.png");
	Texture2D geoSmashImg = LoadTexture("./assets/GeoSmash.png");
	Texture2D riskOfImg = LoadTexture("./assets/RiskOfGettingTrailblaze.png");
	Rectangle gamePanel = { winWidth - 400, 0, 400, winHeight };

	// input start
	bool done = false;
	float progress = 0;
	int state = 0;
	bool showGames = false;
	bool showServer = false;
	bool popupActive = false;

	bool compressMode = true;
	bool oldCompressMode = true;
	int maxOrder = 0;

	char textFilePath[300] = { '\0' };
	char treeFilePath[300] = { '\0' };
	char dirPath[300] = { '\0' };

	char generatedFilePath[300] = { '\0' };
	char generatedFileName[300] = { '\0' };
	float generatedFileValue = 0;
	bool orderInputActive = false;
	// input end

	const char* filterTreePatterns[1] = { "*.json" };
	const char* filterBinPatterns[1] = { "*.bin" };
	const char* filterTextPatterns[1] = { "*.txt" };
	const char* selectedTextFile = nullptr;

	SetTargetFPS(60);
	GuiLoadStyle("style_terminal.rgs");
	GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

		GuiToggle(Rectangle{ 20, 20, 120, 30 }, "Mode: ", &compressMode);
		if (oldCompressMode != compressMode) {
			strcpy(textFilePath, "\0");
			strcpy(treeFilePath, "\0");
			oldCompressMode = compressMode;
		}

		GuiLabel(Rectangle{ 20, 60, winWidth, 100 }, compressMode ? "#18# File to Compress" : "#200# File to Decompress");
		if (GuiTextBox(Rectangle{ 50, 160, winWidth * 3 / 4, 40 }, textFilePath, sizeof(textFilePath), false)
			|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 160, 40, 40 }, "#5#")) {
			if (compressMode) {
				selectedTextFile = selectFileDialog("Select a file", NULL, 1, filterTextPatterns, "Text Files");

				if (selectedTextFile != NULL) {
					strncpy(textFilePath, selectedTextFile, strlen(selectedTextFile));
					textFilePath[strlen(selectedTextFile)] = '\0';

					for (int i = 0; textFilePath[i] != '\0'; i++) {
						if (textFilePath[i] == '\\') {
							textFilePath[i] = '/';
						}
					}
				}
			}
			else {
				selectedTextFile = selectFileDialog("Select a file", NULL, 1, filterBinPatterns, "Text Files");
				if (selectedTextFile != NULL) {
					strncpy(textFilePath, selectedTextFile, strlen(selectedTextFile));
					textFilePath[strlen(selectedTextFile)] = '\0';

					for (int i = 0; textFilePath[i] != '\0'; i++) {
						if (textFilePath[i] == '\\') {
							textFilePath[i] = '/';
						}
					}
				}
			}
		}

		if (compressMode) {
			GuiLabel(Rectangle{ 150, 20, 200, 30 }, "#37# Compress");

			GuiLabel(Rectangle{ 20, 230, winWidth, 100 }, "#188# Max Order of compression");
			if (GuiValueBox(Rectangle{ 350, 267.5, 100, 35 }, NULL, &maxOrder, 1, 100, orderInputActive)) {
				orderInputActive = true;
				showServer = false;
			}
			GuiLabel(Rectangle{ 50, 280, winWidth, 100 }, "#191# Order of compression is how many nested compression operations should be taken");
			GuiLabel(Rectangle{ 50, 320, winWidth, 100 }, "#191# Note that if the size difference is less than 5kb, it will stop despite reaching the desired order or not");
		}
		else {
			GuiLabel(Rectangle{ 150, 20, 200, 30 }, "#33# Decompress");

			GuiLabel(Rectangle{ 20, 230, winWidth, 100 }, "#138# Tree Path");
			if (GuiTextBox(Rectangle{ 50, 330, winWidth * 3 / 4, 40 }, treeFilePath, sizeof(treeFilePath), false)
				|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 330, 40, 40 }, "#5#")) {
				selectedTextFile = selectFileDialog("Select a file", NULL, 1, filterTreePatterns, "Text Files");
				if (selectedTextFile != NULL) {
					strncpy(treeFilePath, selectedTextFile, strlen(selectedTextFile));
					treeFilePath[strlen(selectedTextFile)] = '\0';

					for (int i = 0; treeFilePath[i] != '\0'; i++) {
						if (treeFilePath[i] == '\\') {
							treeFilePath[i] = '/';
						}
					}
				}
			}
		}

		GuiLabel(Rectangle{ 20, 400, winWidth, 100 }, "#217# Output Directory");
		if (GuiTextBox(Rectangle{ 50, 500, winWidth * 3 / 4, 40 }, dirPath, sizeof(dirPath), false)
			|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 500, 40, 40 }, "#5#")) {
			const char* selectedPath = selectFolderDialog("Select a folder", NULL);
			if (selectedPath != NULL) {
				strncpy(dirPath, selectedPath, strlen(selectedPath));
				dirPath[strlen(selectedPath)] = '\0';


				for (int i = 0; dirPath[i] != '\0'; i++)
					if (dirPath[i] == '\\')
						dirPath[i] = '/';
			}
		}

		panelViewer(&popupActive, &state, &showGames, &showServer, generatedFilePath, filterTextPatterns, &generatedFileValue, &orderInputActive, gamePanel, winWidth, animVsWitherImg, geoSmashImg, riskOfImg);

		if (GuiButton(Rectangle{ winWidth / 2 - 150, winHeight - 60, 300, 40 }, "Start")) {
			std::thread workThread;

			if (compressMode)
				workThread = std::thread(startCompressing, std::string(textFilePath), dirPath, maxOrder <= 0 ? 1 : maxOrder, &done, &state, &progress);
			else
				workThread = std::thread(StartDecompressing, std::string(textFilePath), treeFilePath, dirPath, &done, &state, &progress);

			workThread.detach();

			while (!done) {
				BeginDrawing();
				ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
				GuiProgressBar(Rectangle{ winWidth / 2 - 200, winHeight / 2 - 20, 400, 40 }, NULL, NULL, &progress, 0, 1);
				panelViewer(&popupActive, &state, &showGames, &showServer, generatedFilePath, filterTextPatterns, &generatedFileValue, &orderInputActive, gamePanel, winWidth, animVsWitherImg, geoSmashImg, riskOfImg);
				EndDrawing();
			}

			selectedTextFile = nullptr;
			strcpy(textFilePath, "\0");
			strcpy(treeFilePath, "\0");
			popupActive = true;
			done = false;
			progress = 0;
		}

		if (popupActive) {
			if (state == 0) {
				if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "SUCCESS", "Done!", "OK") >= 0)
					popupActive = false;
			}
			else if (state == 1) {
				if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "ERROR", "Failed to read/create file", "OK") >= 0)
					popupActive = false;
			}
			else if (state == -1) {
				if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "ERROR", "Fatal Error", "Crap") >= 0)
					popupActive = false;
			}
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}