#include "RayMainFrame.h"
#include <direct.h>
#include "raylib.h"
#include "StartCompressing.h"
#include "StartDecompressing.h"
#include "utils.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int RayMainFrame() {
	const int fontSize = 30;
	const float winWidth = 1280;
	const float winHeight = 720;
	InitWindow(winWidth, winHeight, "3A-Z");


	Texture2D animVsWither = LoadTexture("./assets/AnimationVsWither.png");
	Rectangle gamePanel = { winWidth - 400, 0, 400, winHeight };

	// input start
	bool showGames = false;
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

		if (showGames) {
			GuiPanel(gamePanel, "Play something while waiting");

			if (GuiButton(Rectangle{ winWidth - 380, 30, 150, 150 }, "")) {
				runExecutable(".\\games\\AnimationVsWither\\main.exe");
			}
			DrawTextureRec(animVsWither, Rectangle{ 0,0, 150, 150 }, Vector2{ winWidth - 380, 30 }, WHITE);


		}
		GuiToggle(Rectangle{ winWidth - 40, 0, 40, 40 }, showGames ? "#128#" : "#152#", &showGames);


		if (GuiButton(Rectangle{ winWidth / 2 - 150, winHeight - 60, 300, 40 }, "Start")) {
			if (compressMode) {
				// thread this fucker first
				startCompressing(textFilePath, dirPath, maxOrder <= 0 ? 1 : maxOrder);
			}
			else {
				StartDecompressing(textFilePath, treeFilePath, dirPath);
			}
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}