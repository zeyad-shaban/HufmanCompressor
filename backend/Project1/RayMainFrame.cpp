#include "RayMainFrame.h"

int RayMainFrame() {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "3A-Z");

	Vector2 circlePosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
	float radius = 50.0f;

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawCircleV(circlePosition, radius, WHITE);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}