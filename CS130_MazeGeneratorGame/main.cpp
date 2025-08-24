#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

static const float PLAYER_EYE_HEIGHT = 1.7f;   
static const float TOPDOWN_DISTANCE = 35.0f;  
static const float ORTHO_FOV = 20.0f;  
static const float PERS_FOV = 60.0f;

Camera camera;
int cameraMode = CAMERA_FIRST_PERSON;
Vector3 playerPos = { 0, 0, 0 };
Vector3 fpForward = { 0, 0, 1.0f };

static inline Vector3 CameraForward(const Camera* c) {
	return Vector3Normalize(Vector3Subtract(c->target, c->position));
}

void ToggleCamera() {
	if (IsKeyPressed(KEY_TAB)) {
		if (cameraMode == CAMERA_FIRST_PERSON) {
			fpForward = CameraForward(&camera);
			if (Vector3Length(fpForward) < 0.0001f)
				fpForward = { 0, 0, 1 };

			cameraMode = CAMERA_THIRD_PERSON;

			camera.projection = CAMERA_ORTHOGRAPHIC;
			camera.fovy = ORTHO_FOV;
			camera.up = { 0.0f, 0.0f, -1.0f };

			camera.position = { playerPos.x, playerPos.y + TOPDOWN_DISTANCE, playerPos.z };
			camera.target = playerPos;
		}
		else if (cameraMode == CAMERA_THIRD_PERSON) {
			cameraMode = CAMERA_FIRST_PERSON;

			camera.projection = CAMERA_PERSPECTIVE;
			camera.fovy = PERS_FOV;
			camera.up = { 0.0f, 1.0f, 0.0f };
			camera.position = { playerPos.x, playerPos.y + PLAYER_EYE_HEIGHT, playerPos.z };
			camera.target = Vector3Add(camera.position, fpForward);
		}
	}
}

void DrawPlayerInThirdPerson() {
	if (cameraMode == CAMERA_THIRD_PERSON)
	{
		DrawSphere(playerPos, 0.3f, PURPLE);
		DrawSphereWires(playerPos, 0.3f, 8, 8, DARKPURPLE);
	}
}

void SyncCameraToPlayer() {
	if (cameraMode == CAMERA_FIRST_PERSON) {
		playerPos.x = camera.position.x;
		playerPos.z = camera.position.z;
		playerPos.y = 0.0f;

		camera.position.y = playerPos.y + PLAYER_EYE_HEIGHT;
	}
	else if (cameraMode == CAMERA_THIRD_PERSON) {
		camera.fovy = ORTHO_FOV;
		camera.position = { playerPos.x, playerPos.y + TOPDOWN_DISTANCE, playerPos.z };;
		camera.target = playerPos;
	}
}

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "Maze Escape Game");
	SetTargetFPS(60);

	DisableCursor();

	camera = { 0 };
	camera.position = { playerPos.x, playerPos.y + PLAYER_EYE_HEIGHT, playerPos.z - 4.0f };
	camera.target = { playerPos.x, playerPos.y + PLAYER_EYE_HEIGHT, playerPos.z };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = PERS_FOV;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 cubePos = { 0, 2, 0 };

	//Main Game Loop
	while (!WindowShouldClose()) {
		ToggleCamera();

		if (cameraMode == CAMERA_FIRST_PERSON) {
			UpdateCamera(&camera, CAMERA_FIRST_PERSON);
			fpForward = CameraForward(&camera);
			if (Vector3Length(fpForward) < 0.0001f)
				fpForward = { 0, 0, 1 };
		}

		SyncCameraToPlayer();
		BeginDrawing();

				ClearBackground(RAYWHITE);
				BeginMode3D(camera);
				DrawCube(cubePos, 2.0f, 4.0f, 2.0f, RED);
				DrawCubeWires(cubePos, 2.0f, 4.0f, 2.0f, MAROON);

				DrawPlayerInThirdPerson();
				DrawGrid(100, 1.0f);
			EndMode3D();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}