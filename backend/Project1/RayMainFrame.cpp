#include "RayMainFrame.h"
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

void panelViewer(bool* showGames, bool* showServer, Rectangle gamePanel, float winWidth, Texture2D animVsWitherImg, Texture2D geoSmashImg, Texture2D riskOfImg) {
	if (*showGames) {
		GuiPanel(gamePanel, "Play something while waiting");

		if (GuiButton(Rectangle{ winWidth - 395, 40, 190, 190 }, "")) {
			runExecutable(".\\games\\AnimationVsWither\\main.exe");
		}
		DrawTextureRec(animVsWitherImg, Rectangle{ 0,0, 190, 190 }, Vector2{ winWidth - 395, 40 }, WHITE);

		if (GuiButton(Rectangle{ winWidth - 200, 40, 200, 200 }, "")) {
			runExecutable(".\\games\\GeoSmash\\main.exe");
		}
		DrawTextureRec(geoSmashImg, Rectangle{ 0,0, 200, 200 }, Vector2{ winWidth - 200, 40 }, WHITE);

		if (GuiButton(Rectangle{ winWidth - 395, 200, 200, 200 }, "")) {
			runExecutable(".\\games\\RiskOfGettingTrailblaze\\Risk of getting Railed.exe");
		}
		DrawTextureRec(riskOfImg, Rectangle{ 0,0, 200, 200 }, Vector2{ winWidth - 395, 200 }, WHITE);

	}
	GuiToggle(Rectangle{ winWidth - 40, 0, 40, 40 }, *showGames ? "#128#" : "#152#", showGames);

	if (*showServer) {
		GuiPanel(gamePanel, "Open Server");
		// Add your server related code here
	}
	GuiToggle(Rectangle{ winWidth - 80, 0, 40, 40 }, *showServer ? "#128#" : "#198#", showServer);
}



int RayMainFrame() {
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
	// input end

	const char* filterTreePatterns[1] = { "*.json" };
	const char* filterBinPatterns[1] = { "*.bin" };
	const char* filterTextPatterns[1] = { "*.txt" };
	const char* selectedTextFile = nullptr;

	SetTargetFPS(60);
	GuiLoadStyle("style_terminal.rgs");
	GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

		GuiToggle(Rectangle{ 20, 20, 120, 30 }, "Mode: ", &compressMode);
		if (oldCompressMode != compressMode) {
			strcpy(textFilePath, "\0");
			oldCompressMode = compressMode;
		}


		GuiLabel(Rectangle{ 20, 60, winWidth,100 }, compressMode ? "#18# File to Compress" : "#200# File to Decompress");
		if (GuiTextBox(Rectangle{ 50, 160, winWidth * 3 / 4, 40 }, textFilePath, sizeof(textFilePath), false)
			|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 160, 40, 40 }, "#5#")) {
			if (compressMode) {
				selectedTextFile = selectFileDialog("Select a file", NULL, 1, filterTextPatterns, "Text Files");

				if (selectedTextFile != NULL) {
					strncpy(textFilePath, selectedTextFile, sizeof(textFilePath) - 1);

					for (int i = 0; textFilePath[i] != '\0'; i++) {
						if (textFilePath[i] == '\\') {
							textFilePath[i] = '/';
						}
					}
				}
				selectedTextFile = nullptr;
			}
			else {
				selectedTextFile = selectFileDialog("Select a file", NULL, 1, filterBinPatterns, "Text Files");
				if (selectedTextFile != NULL) {
					strncpy(textFilePath, selectedTextFile, sizeof(textFilePath) - 1);
					textFilePath[sizeof(textFilePath) - 1] = '\0';

					for (int i = 0; textFilePath[i] != '\0'; i++) {
						if (textFilePath[i] == '\\') {
							textFilePath[i] = '/';
						}
					}
				}
				selectedTextFile = nullptr;
			}
		}

		if (compressMode) {
			GuiLabel(Rectangle{ 150, 20, 200, 30 }, "#37# Compress");

			GuiLabel(Rectangle{ 20, 230, winWidth,100 }, "#188# Max Order of compression");
			GuiValueBox(Rectangle{ 350, 267.5, 100, 35 }, NULL, &maxOrder, 1, 100, true);
			GuiLabel(Rectangle{ 50, 280, winWidth, 100 }, "#191# Order of compression is how much nested compression operations should be taken");
			GuiLabel(Rectangle{ 50, 320, winWidth, 100 }, "#191# note that if the size difference was less than 5kb it will stop despite reaching the desired order or not");
		}
		else {
			GuiLabel(Rectangle{ 150, 20, 200, 30 }, "#33# Decompress");

			GuiLabel(Rectangle{ 20, 230, winWidth,100 }, "#138# Tree Path");
			if (GuiTextBox(Rectangle{ 50, 330, winWidth * 3 / 4, 40 }, treeFilePath, sizeof(treeFilePath), false)
				|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 330, 40, 40 }, "#5#")) {
				selectedTextFile = selectFileDialog("Select a file", NULL, 1, filterTreePatterns, "Text Files");
				if (selectedTextFile != NULL) {
					strncpy(treeFilePath, selectedTextFile, sizeof(treeFilePath) - 1);
					treeFilePath[sizeof(treeFilePath) - 1] = '\0';

					for (int i = 0; treeFilePath[i] != '\0'; i++) {
						if (treeFilePath[i] == '\\') {
							treeFilePath[i] = '/';
						}
					}
				}
				selectedTextFile = nullptr;
			}
		}

		GuiLabel(Rectangle{ 20, 400, winWidth,100 }, "#217# Output Directory");
		if (GuiTextBox(Rectangle{ 50, 500, winWidth * 3 / 4, 40 }, dirPath, sizeof(dirPath), false)
			|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 500, 40, 40 }, "#5#")) {
			const char* selectedPath = selectFolderDialog("Select a folder", NULL);
			if (selectedPath != NULL) {
				strncpy(dirPath, selectedPath, sizeof(dirPath) - 1);
				dirPath[sizeof(dirPath) - 1] = '\0'; // Ensure null termination

				// Replace backslashes with forward slashes
				for (int i = 0; dirPath[i] != '\0'; i++) {
					if (dirPath[i] == '\\') {
						dirPath[i] = '/';
					}
				}

				printf("%s", dirPath);
			}
		}

		panelViewer(&showGames, &showServer, gamePanel, winWidth, animVsWitherImg, geoSmashImg, riskOfImg);


		if (GuiButton(Rectangle{ winWidth / 2 - 150, winHeight - 60, 300, 40 }, "Start")) {
			std::thread workThread;

			if (compressMode)
				workThread = std::thread(startCompressing, std::string(textFilePath), dirPath, maxOrder <= 0 ? 1 : maxOrder, &done, &state, &progress);
			else
				workThread = std::thread(StartDecompressing, std::string(textFilePath), treeFilePath, dirPath, &done, &state, &progress);

			while (!done) {
				BeginDrawing();
				ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
				GuiProgressBar(Rectangle{ winWidth / 2 - 200, winHeight / 2 - 20, 400, 40 }, NULL, NULL, &progress, 0, 1);
				panelViewer(&showGames, &showServer, gamePanel, winWidth, animVsWitherImg, geoSmashImg, riskOfImg);
				EndDrawing();
			}

			workThread.join();
			popupActive = true;
			done = false;
			progress = 0;
			state = 0;
		}

		if (popupActive) {
			if (state == 0) if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "SUCCESS", "Done!", "OK") >= 0) { popupActive = false; }
			else if (state == 1) if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "ERROR", "Failed to read/create file, please validate path", "OK") >= 0) { popupActive = false; }
			else if (state == -1) if (GuiMessageBox(Rectangle{ winWidth / 2 - 210, winHeight / 2 - 100, 420, 200 }, "ERROR", "Fatal Error", "Crap") >= 0) { popupActive = false; }
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}