#include "RayMainFrame.h"
#include <direct.h>
#include "raylib.h"
#include "StartCompressing.h"
#include "StartDecompressing.h"
#include "utils.h"
#include <thread>
#include <future>


#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void gameViewer(bool* showGames, Rectangle gamePanel, float winWidth, Texture2D animVsWitherImg, Texture2D geoSmashImg) {
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

	}
	GuiToggle(Rectangle{ winWidth - 40, 0, 40, 40 }, *showGames ? "#128#" : "#152#", showGames);
}


int RayMainFrame() {
	const int fontSize = 30;
	const float winWidth = 1280;
	const float winHeight = 720;
	InitWindow(winWidth, winHeight, "3A-Z");


	Texture2D animVsWitherImg = LoadTexture("./assets/AnimationVsWither.png");
	Texture2D geoSmashImg = LoadTexture("./assets/GeoSmash.png");
	Rectangle gamePanel = { winWidth - 400, 0, 400, winHeight };

	// input start
	bool done = false;
	float progress = 0;
	int state = 0;
	bool showGames = false;
	bool popupActive = false;

	bool compressMode = true;
	int maxOrder = 0;

	char textFilePath[300] = { '\0' };
	char treeFilePath[300] = { '\0' };
	char dirPath[300] = { '\0' };
	// input end

	SetTargetFPS(60);
	GuiLoadStyle("style_terminal.rgs");
	GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

		GuiToggle(Rectangle{ 20, 20, 120, 30 }, "Mode: ", &compressMode);

		GuiLabel(Rectangle{ 20, 60, winWidth,100 }, compressMode ? "#18# File to Compress" : "#200# File to Decompress");
		if (GuiTextBox(Rectangle{ 50, 160, winWidth * 3 / 4, 40 }, textFilePath, sizeof(textFilePath), false)
			|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 160, 40, 40 }, "#5#")) {
			strcpy(textFilePath, "C:/Users/zeyad/OneDrive/Desktop/test/test.txt"); // TODO OPEN DIALOG HERE INSTEAD
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
				strcpy(treeFilePath, "C:/Users/zeyad/OneDrive/Desktop/test/test_tree.json"); // TODO OPEN DIALOG HERE INSTEAD
			}
		}

		GuiLabel(Rectangle{ 20, 400, winWidth,100 }, "#217# Output Directory");
		if (GuiTextBox(Rectangle{ 50, 500, winWidth * 3 / 4, 40 }, dirPath, sizeof(dirPath), false)
			|| GuiButton(Rectangle{ (winWidth * 3 / 4) + 50, 500, 40, 40 }, "#5#")) {
			strcpy(dirPath, "C:/Users/zeyad/OneDrive/Desktop/test"); // TODO OPEN DIALOG HERE INSTEAD
		}

		gameViewer(&showGames, gamePanel, winWidth, animVsWitherImg, geoSmashImg);


		if (GuiButton(Rectangle{ winWidth / 2 - 150, winHeight - 60, 300, 40 }, "Start")) {
			if (compressMode) {
				std::thread compressionThread(startCompressing, textFilePath, dirPath, maxOrder <= 0 ? 1 : maxOrder, &done, &state, &progress);

				while (true) {
					BeginDrawing();
					ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
					GuiProgressBar(Rectangle{ winWidth / 2 - 200, winHeight / 2 - 20, 400, 40 }, NULL, NULL, &progress, 0, 1);
					gameViewer(&showGames, gamePanel, winWidth, animVsWitherImg, geoSmashImg);

					if (done) {
						compressionThread.join();
						break;
					}
					EndDrawing();
				}

				popupActive = true;
				done = false;
				progress = 0;
				state = 0;
			}
			else {
				StartDecompressing(textFilePath, treeFilePath, dirPath);
			}
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