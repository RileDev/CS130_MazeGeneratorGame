#include "include/raylib.h"
#include "include/raymath.h"
#include <cmath>

class Cat {
private:
	Model model;
	Vector3 pos;
	Vector3 basePos;
	float scale = 1.0f;
	float yaw = 0.0f;
	bool loaded = false;

	float yawSpeedDeg = 45.0f;
	float bobAmp = .5f;   
	float bobHz = .5f;  
	float t = 0.0f;

	float baseRadiusXZ = 0.5f;

public:
	Cat(const char* modelPath = "./assets/models/scene.gltf",
		Vector3 pos = { 0, 1, 0 },
		float scale = 1.0f,
		float yawDeg = 0.0f)
		: pos(pos), basePos(pos), scale(scale), yaw(yawDeg)
	{
		model = LoadModel(modelPath);
		loaded = (model.meshCount > 0);

		if (loaded) {
			BoundingBox bb = GetModelBoundingBox(model);
			float rx = (bb.max.x - bb.min.x) * 0.5f;
			float rz = (bb.max.z - bb.min.z) * 0.5f;
			baseRadiusXZ = sqrt(rx * rx + rz * rz);
		}
		
	}

	Cat(const Cat&) = delete;
	Cat& operator=(const Cat&) = delete;

	void DrawCat(Color tint = WHITE) const {
		if (!loaded) return;
		DrawModelEx(model, pos, { 0.0f, 1.0f, 0.0f }, yaw,
			{ scale, scale, scale }, tint);
	}

	void Update(float dt) {
		if (!loaded) return;
		t += dt;
		yaw = std::fmod(yaw + yawSpeedDeg * dt, 360.0f);          
		pos.y = (basePos.y + std::sinf(t * 2.0f * PI * bobHz) * bobAmp) - .25f;
	}

	bool IsColliding(Vector3& playerPosition, float playerRadius, float inflate = 0.0) const {
		if (!loaded) return false;
		float dx = playerPosition.x - pos.x;
		float dz = playerPosition.z - pos.z;
		float dist2 = dx * dx + dz * dz;

		float catRadius = baseRadiusXZ * scale;
		float sum = playerRadius + catRadius + inflate;
		return dist2 <= (sum * sum);
	}

	~Cat(){
		if(loaded)
			UnloadModel(model);
	}
};