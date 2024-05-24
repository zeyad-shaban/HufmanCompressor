#include <direct.h>
#include "raylib.h"
#include "StartCompressing.h"
#include "StartDecompressing.h"
#include "utils.h"
#include <thread>
#include <future>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void panelViewer(bool* popupActive, int* state, bool* showGames, bool* showServer, char* generatedFileName, float* generatedVal, char* pathA, char* pathB, bool* orderInputActive, Rectangle gamePanel, float winWidth, Texture2D animVsWitherImg, Texture2D geoSmashImg, Texture2D riskOfImg) {
	if (*showGames && *showServer) *showGames = *showServer = false;

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
	else if (*showServer) {
		*orderInputActive = false;
		GuiPanel(gamePanel, "Tools");

		// =========== FILE FILLER =============
		GuiLabel(Rectangle{ winWidth - 395, 30, winWidth, 100 }, "#8# File Filler");
		GuiLabel(Rectangle{ winWidth - 375, 80, winWidth, 100 }, "#217# Path: ");
		if (GuiTextBox(Rectangle{ winWidth - 230, 115, 150, 40 }, generatedFileName, sizeof(generatedFileName), false)
			|| GuiButton(Rectangle{ winWidth - 230 + 150, 115, 40, 40 }, "#5#")) {

			OpenFileDialog(L"Text Files\0*.txt\0\0", generatedFileName);
		}

		char label[50];
		sprintf(label, "#218# Size in GB: %.2f", *generatedVal);
		GuiLabel(Rectangle{ winWidth - 375, 130, winWidth, 100 }, label);

		GuiSliderBar(Rectangle{ winWidth - 375, 220, 350, 20 }, "", NULL, generatedVal, 0.0f, 100.0f);

		if (GuiButton(Rectangle{ winWidth - 350, 270, 300, 40 }, "Fill")) {
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

		// ========== IDENTICAL CHECKER =============
		GuiLabel(Rectangle{ winWidth - 395, 380, winWidth, 100 }, "#8# File Identical Checker");
		GuiLabel(Rectangle{ winWidth - 375, 440, winWidth, 100 }, "#217# Path A: ");
		if (GuiTextBox(Rectangle{ winWidth - 230, 475, 150, 40 }, pathA, sizeof(pathA), false)
			|| GuiButton(Rectangle{ winWidth - 230 + 150, 475, 40, 40 }, "#5#")) {

			OpenFileDialog(L"Text Files\0*.txt\0\0", pathA);
		}

		GuiLabel(Rectangle{ winWidth - 375, 510, winWidth, 100 }, "#217# Path B: ");
		if (GuiTextBox(Rectangle{ winWidth - 230, 545, 150, 40 }, pathB, sizeof(pathB), false)
			|| GuiButton(Rectangle{ winWidth - 230 + 150, 545, 40, 40 }, "#5#")) {

			OpenFileDialog(L"Text Files\0*.txt\0\0", pathB);
		}

		if (GuiButton(Rectangle{ winWidth - 350, 630, 300, 40 }, "Check")) {
			bool done = false;
			float progress = 0;

			std::thread worker(threadFileComparator, pathA, pathB, &done, state, &progress);
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

	GuiToggle(Rectangle{ winWidth - 40, 0, 40, 40 }, *showGames ? "#128#" : "#152#", showGames);
	GuiToggle(Rectangle{ winWidth - 80, 0, 40, 40 }, *showServer ? "#128#" : "#140#", showServer);
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
	char pathA[300] = { '\0' };
	char pathB[300] = { '\0' };
	float generatedFileValue = 0;
	bool orderInputActive = false;
	// input end

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
			if (compressMode)
				OpenFileDialog(L"Text Files\0*.txt\0\0", textFilePath);
			else
				OpenFileDialog(L"Binary Files\0*.bin\0\0", textFilePath);
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

				OpenFileDialog(L"JSON Files\0*.json\0\0", treeFilePath);
			}
		}

		GuiLabel(Rectangle{ 20, 400, winWidth, 100 }, "#217# Output Directory");
		if (GuiTextBox(Rectangle{ 50, 500, winWidth * 3 / 4, 40 }, dirPath, sizeof(dirPath), false)
			|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 500, 40, 40 }, "#5#")) {

			OpenDirectoryDialog(dirPath);
		}

		panelViewer(&popupActive, &state, &showGames, &showServer, generatedFilePath, &generatedFileValue, pathA, pathB, &orderInputActive, gamePanel, winWidth, animVsWitherImg, geoSmashImg, riskOfImg);

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
				panelViewer(&popupActive, &state, &showGames, &showServer, generatedFilePath, &generatedFileValue, pathA, pathB, &orderInputActive, gamePanel, winWidth, animVsWitherImg, geoSmashImg, riskOfImg);
				EndDrawing();
			}

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
				if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "ERROR", "Fatal Memory Error", "Crap") >= 0)
					popupActive = false;
			}
			else if (state == 2) { // files equal during comparison
				if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "SUCCESS", "Files are identical", "LETS GOO") >= 0)
					popupActive = false;
			}
			else if (state == 3) { // files were not identical during comparison
				if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "ERROR", "Files are NOT identical", "Crap") >= 0)
					popupActive = false;
			}
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}