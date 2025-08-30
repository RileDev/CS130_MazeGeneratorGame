#include "raylib.h"
#include "raymath.h"
#include "Maze.cpp"

const float CELL = 2.0f;
const float WALL_THICK = .2f;
const float WALL_HEIGHT = 2.0f;
const float GROUND_Y = 0.0f;
const float FLOOR_THICK = 0.12f;
const Vector3 ORIGIN = { -(SIZE * CELL) * 0.5f, GROUND_Y, -(SIZE * CELL) * 0.5f };

class Maze3D {
private:
	Maze maze;
	bool built = false;
	std::vector<BoundingBox> boxes;

	static BoundingBox MakeBox(Vector3 c, float sx, float sy, float sz) {
		Vector3 h = { sx * 0.5f, sy * 0.5f, sz * 0.5f };
		return { { c.x - h.x, c.y - h.y, c.z - h.z },
				 { c.x + h.x, c.y + h.y, c.z + h.z } };
	}

protected:
	float centerX(int i) {
		return ORIGIN.x + i * CELL + CELL * 0.5;	
	}

	float centerZ(int j) {
		return ORIGIN.z + j * CELL + CELL * 0.5;
	}

	float edgeX(int i) {
		return ORIGIN.x + i * CELL;
	}

	float edgeZ(int j) {
		return ORIGIN.z + j * CELL;
	}

public:
	Maze3D(Maze maze) : maze(maze) {}

	void drawWallRightEdge(int i, int j, Color fill = RED, Color wire = MAROON) {
		Vector3 pos = { edgeX(i + 1), GROUND_Y + WALL_HEIGHT * 0.5f, centerZ(j) };
		DrawCube(pos, WALL_THICK, WALL_HEIGHT, CELL, fill);    
		DrawCubeWires(pos, WALL_THICK, WALL_HEIGHT, CELL, wire);
	}

	void drawWallBottomEdge(int i, int j, Color fill = RED, Color wire = MAROON) {
		Vector3 pos = { centerX(i), GROUND_Y + WALL_HEIGHT * 0.5f, edgeZ(j + 1) };
		DrawCube(pos, CELL, WALL_HEIGHT, WALL_THICK, fill); 
		DrawCubeWires(pos, CELL, WALL_HEIGHT, WALL_THICK, wire);
	}

	void drawWallLeftBorder(int j, Color fill = RED, Color wire = MAROON) {
		Vector3 pos = { edgeX(0), GROUND_Y + WALL_HEIGHT * 0.5f, centerZ(j) };
		DrawCube(pos, WALL_THICK, WALL_HEIGHT, CELL, fill);
		DrawCubeWires(pos, WALL_THICK, WALL_HEIGHT, CELL, wire);
	}

	void drawWallTopBorder(int i, Color fill = RED, Color wire = MAROON) {
		Vector3 pos = { centerX(i), GROUND_Y + WALL_HEIGHT * 0.5f, edgeZ(0) };
		DrawCube(pos, CELL, WALL_HEIGHT, WALL_THICK, fill);
		DrawCubeWires(pos, CELL, WALL_HEIGHT, WALL_THICK, wire);
	}

	void drawStartFloor(int i, Color fill = GREEN, Color wire = MAROON) {
		Vector3 pos = { centerX(i), GROUND_Y - FLOOR_THICK * 0.5f, centerZ(i) };
		DrawCube(pos, CELL, FLOOR_THICK, CELL, fill); 
		DrawCubeWires(pos, CELL, FLOOR_THICK, CELL, wire);
	}

	void drawEndFloor(Color fill = PURPLE, Color wire = MAROON) {
		int x = SIZE - 1, y = SIZE - 1;
		Vector3 pos = { centerX(x), GROUND_Y - FLOOR_THICK * 0.5f, centerZ(y) };
		DrawCube(pos, CELL, FLOOR_THICK, CELL, fill);
		DrawCubeWires(pos, CELL, FLOOR_THICK, CELL, wire);
	}



	void build() {
		if (this->built) return;
		maze.createMazeDfs();
		boxes.clear();
		boxes.reserve(SIZE * SIZE * 2 + SIZE * 2); 

		for (int j = 0; j < SIZE; ++j) {
			Vector3 c = { edgeX(0), GROUND_Y + WALL_HEIGHT * 0.5f, centerZ(j) };
			boxes.push_back(MakeBox(c, WALL_THICK, WALL_HEIGHT, CELL));
		}
		for (int i = 0; i < SIZE; ++i) {
			Vector3 c = { centerX(i), GROUND_Y + WALL_HEIGHT * 0.5f, edgeZ(0) };
			boxes.push_back(MakeBox(c, CELL, WALL_HEIGHT, WALL_THICK));
		}

		for (int j = 0; j < SIZE; ++j) {
			for (int i = 0; i < SIZE; ++i) {
				if (i == SIZE - 1 || !maze.rightOpen(i, j)) {
					Vector3 c = { edgeX(i + 1), GROUND_Y + WALL_HEIGHT * 0.5f, centerZ(j) };
					boxes.push_back(MakeBox(c, WALL_THICK, WALL_HEIGHT, CELL));
				}
				if (j == SIZE - 1 || !maze.downOpen(i, j)) {
					Vector3 c = { centerX(i), GROUND_Y + WALL_HEIGHT * 0.5f, edgeZ(j + 1) };
					boxes.push_back(MakeBox(c, CELL, WALL_HEIGHT, WALL_THICK));
				}
			}
		}

		built = true;
	}

	const std::vector<BoundingBox>& colliders() const 
	{ 
		return boxes; 
	}


	void draw() {
		drawStartFloor(maze.getStartPos());

		for (int j = 0; j < SIZE; j++) {
			drawWallLeftBorder(j);
		}

		for (int i = 0; i < SIZE; i++) {
			drawWallTopBorder(i);
		}

		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (i == SIZE - 1 || !maze.rightOpen(i, j)) {
					drawWallRightEdge(i, j);
				}

				if (j == SIZE - 1 || !maze.downOpen(i, j)) {
					drawWallBottomEdge(i, j);
				}
			}
		}

		drawEndFloor();

	}
};