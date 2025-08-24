#include "raylib.h"
#include "rcamera.h"

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "Maze Escape Game");
	SetTargetFPS(60);

	Camera camera = { 0 };
	camera.position = { 0.0f, 10.0f, 10.0f };
	camera.target = { 0.0f, 0.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 cubePos = { 0, 0, 0 };

	//Main Game Loop
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(SKYBLUE);

		BeginMode3D(camera);

		DrawCube(cubePos, 2.0f, 2.0f, 2.0f, RED);
		DrawCubeWires(cubePos, 2.0f, 2.0f, 2.0f, MAROON);

		EndMode3D();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}