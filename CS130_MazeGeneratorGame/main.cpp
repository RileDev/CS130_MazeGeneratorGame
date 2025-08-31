#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "Maze3D.cpp"
#include "CountdownTimer.cpp"

static const float PLAYER_EYE_HEIGHT = 1.7f;   
static const float TOPDOWN_DISTANCE = 35.0f;  
static const float ORTHO_FOV = 20.0f;  
static const float PERS_FOV = 60.0f;

Maze3D maze = Maze3D(Maze());
Texture2D arrow;

Camera camera;
int cameraMode = CAMERA_FIRST_PERSON;
Vector3 playerPos = { -19, 0, -15 };
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
			EnableCursor();
		}
		else if (cameraMode == CAMERA_THIRD_PERSON) {
			cameraMode = CAMERA_FIRST_PERSON;

			camera.projection = CAMERA_PERSPECTIVE;
			camera.fovy = PERS_FOV;
			camera.up = { 0.0f, 1.0f, 0.0f };
			camera.position = { playerPos.x, playerPos.y + PLAYER_EYE_HEIGHT, playerPos.z };
			camera.target = Vector3Add(camera.position, fpForward);
			DisableCursor();
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

void DrawPlayerArrow2D() {
	if (cameraMode == CAMERA_THIRD_PERSON) {
		Vector2 p2 = GetWorldToScreen({ playerPos.x, playerPos.y, playerPos.z }, camera);

		float scale = 1.0f;
		float w = arrow.width * scale;
		float h = arrow.height * scale;

		float angle = RAD2DEG * atan2f(fpForward.z, fpForward.x) - 90.0f;

		Rectangle src = { 0, 0, (float)arrow.width, (float)arrow.height };
		Rectangle dest = { roundf(p2.x), roundf(p2.y), w, h };
		Vector2 origin = { w * 0.5f, h * 0.5f };
		DrawTexturePro(arrow, src, dest, origin, angle, WHITE);
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

static bool CollidesAt(const Vector3& p, float r, const std::vector<BoundingBox>& boxes) {
	for (const auto& b : boxes)
		if (CheckCollisionBoxSphere(b, p, r)) return true;
	return false;
}

static Vector3 ResolveCollision(const Vector3& from, const Vector3& to, float r,
	const std::vector<BoundingBox>& boxes) {
	if (!CollidesAt(to, r, boxes)) return to;

	Vector3 tryX = { to.x, from.y, from.z };
	if (!CollidesAt(tryX, r, boxes)) return tryX;

	Vector3 tryZ = { from.x, from.y, to.z };
	if (!CollidesAt(tryZ, r, boxes)) return tryZ;

	return from;
}

void checkForGameOver(CountdownTimer timer, int w, int h) {
	if (timer.over()) {
		DrawRectangle(w / 2 - 180, h / 2 - 60, 360, 60, Fade(BLACK, 0.5f));
		DrawRectangleLines(w / 2 - 180, h / 2 - 60, 360, 60, RED);
		DrawText("GAME OVER!", w / 2 - 160, h / 2 - 50, 50, RED);
		EnableCursor();
	}
}


int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "Maze Escape Game");
	SetTargetFPS(60);

	maze.build();
	arrow = LoadTexture("arrow.png");
	const auto& worldBoxes = maze.colliders();
	const float PLAYER_RADIUS = 0.35f;

	DisableCursor();

	camera = { 0 };
	camera.position = { playerPos.x, playerPos.y + PLAYER_EYE_HEIGHT, playerPos.z - 4.0f };
	camera.target = { playerPos.x, playerPos.y + PLAYER_EYE_HEIGHT, playerPos.z };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = PERS_FOV;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 cubePos = { 0, 2, 0 };

	CountdownTimer timer(2, 30);
	timer.start();

	//Main Game Loop
	while (!WindowShouldClose()) {
		ToggleCamera();

		if (cameraMode == CAMERA_FIRST_PERSON) {
			Vector3 prev = camera.position;

			UpdateCamera(&camera, CAMERA_FIRST_PERSON);
			camera.position.y = playerPos.y + PLAYER_EYE_HEIGHT;

			Vector3 corrected = ResolveCollision(prev, camera.position, PLAYER_RADIUS, worldBoxes);

			if (corrected.x != camera.position.x || corrected.z != camera.position.z) {
				Vector3 fwd = CameraForward(&camera);
				camera.position = corrected;
				camera.target = Vector3Add(corrected, fwd);
			}

			fpForward = CameraForward(&camera);
			if (Vector3Length(fpForward) < 0.0001f)
				fpForward = { 0, 0, 1 };
		}

		timer.update();
		SyncCameraToPlayer();
		BeginDrawing();

				ClearBackground(SKYBLUE);
				BeginMode3D(camera);
				
				maze.draw();

				DrawPlayerInThirdPerson();
				DrawPlane({ 0, -0.5f, 0 }, {1000, 1000}, BEIGE);
				//DrawGrid(1000, 1.0f);
			EndMode3D();

			DrawPlayerArrow2D();
			checkForGameOver(timer, screenWidth, screenHeight);
			
			DrawRectangle(10, 10, 150, 60, Fade(SKYBLUE, 0.5f));
			DrawRectangleLines(10, 10, 150, 60, BLUE);
			DrawText("TIMER", 50, 15, 20, BLACK);
			DrawText(timer.getFormatTime().c_str(), 55, 45, 24, timer.over() ? RED : BLACK);

		EndDrawing();
	}

	maze.unload();
	CloseWindow();

	return 0;
}