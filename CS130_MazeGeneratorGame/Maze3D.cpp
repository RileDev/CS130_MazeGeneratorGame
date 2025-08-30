#include "raylib.h"
#include "raymath.h"
#include "Maze.cpp"

const float CELL = 2.0f;
const float WALL_THICK = .2f;
const float WALL_HEIGHT = 2.0f;
const float GROUND_Y = 0.0f;
const Vector3 ORIGIN = { -(SIZE * CELL) * 0.5f, GROUND_Y, -(SIZE * CELL) * 0.5f };

class Maze3D {
private:
	Maze maze;
	bool built = false;

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

	void build() {
		if (this->built) return;
		maze.createMazeDfs();
		built = true;
	}

	void draw() {
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

	}
};