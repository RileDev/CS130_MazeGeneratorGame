# Maze Escape Game — DFS-Generated 3D Maze (C++ & raylib)

A fast, minimal **C++/raylib** project that generates a **perfect maze** using an **iterative Depth-First Search (DFS)** (a.k.a. *recursive backtracker*) and renders it in 3D. You have **2:30** to find the exit — beat the clock or lose the run.

---

## Overview

**Maze Escape Game** is a small teaching project that blends **procedural generation** with **real-time rendering**:
- Implements **perfect-maze** generation (single unique path between any two cells) with an **iterative DFS** using an explicit stack.
- Renders the maze as textured walls and floor tiles in **raylib** with a top-down “map” view toggle for orientation.
- Uses clean, OOP-style separation between the maze model (`Maze`) and the 3D renderer/holder (`Maze3D`).

---

## Game Loop & Objective

- Start at the maze’s origin and **reach the goal** within **2 minutes 30 seconds**.
- If you arrive before time’s up → **Win screen**. Otherwise → **Lose screen**.
- Each run seeds the generator from the current time, producing a **different maze** on (nearly) every start.

---

## Controls

- **W A S D** — move forward/left/back/right  
- **Mouse** — look around (first-person)  
- **Tab** — toggle **top-down** (bird’s-eye) map to help orientation

---

## Screenshots (Placeholders)
- First-person gameplay (corridors & turns)
<img width="801" height="480" alt="Screenshot 2025-09-01 200733" src="https://github.com/user-attachments/assets/75a48bb8-4a16-447b-a578-21e3e169f8bf" />


- Top-down map with player marker
<img width="801" height="482" alt="Screenshot 2025-09-01 201011" src="https://github.com/user-attachments/assets/1dba9d6d-e471-4cbe-8895-13d85cc3c656" />


- **Win** screen
<img width="803" height="479" alt="Screenshot 2025-09-01 201202" src="https://github.com/user-attachments/assets/763683ec-7c5a-4c79-b36c-a69baf76f010" />


- **Lose** screen
<img width="801" height="480" alt="Screenshot 2025-09-01 201539" src="https://github.com/user-attachments/assets/a9893640-6487-45b3-b9f1-9b75bc3e2a36" />

## Why DFS?

---

**Randomized DFS** (recursive backtracker) is simple, fast, and produces aesthetically pleasing layouts:
-Ensures connectivity with no cycles (i.e., a tree over the grid graph → a perfect maze).
-Very compact state: only a visited[] array and an explicit stack.
-Drawback: tends to make long corridors with fewer junctions; that can be desirable for time-challenge gameplay.

Other options you can explore later: Prim, Kruskal, Wilson, Aldous–Broder, Eller, Sidewinder — each yields a distinct “feel”.

---
## Maze Representation
- `SIZE = 20` → grid is `SIZE × SIZE` (e.g., 20×20).
- `visited[SIZE*SIZE] : bool` → flat, linear indices `0 … SIZE*SIZE-1`.
- `walls[SIZE*SIZE][2] : bool` → store **only two openings per cell**:
  - `walls[i][0]` → opening to the right of cell `i`
  - `walls[i][1]` → opening down from cell `i`
  - Left/up openings are **implicit** from neighbors (saves memory and avoids duplication).

**Index helpers**

```cpp
int index(int x, int y)   { return y * SIZE + x; }
int xOf(int i)            { return i % SIZE; }
int yOf(int i)            { return i / SIZE; }
```

**Start position**

`startPos = 0` (top-left). A setter validates input range and falls back to `0` if out of bounds.

---

## DFS Implementation

**Core idea:** iterative DFS with an explicit `std::vector<int> stack`.
1. **nextCell(pos)** — collect valid, unvisited neighbors (up/right/down/left), choose one uniformly at random; return a delta in `{−SIZE, +1, +SIZE, −1}` or `0` if none.
2. **connect(pos1, pos2)** — open the wall between two adjacent cells by setting one of `walls[][0/1]` according to relative position:
    - `pos2 == pos1 + 1` → open **right** at `pos1`
    - `pos2 == pos1 − 1` → open **right at left neighbor**
    - `pos2 == pos1 + SIZE` → open **down** at `pos1`
    - `pos2 == pos1 − SIZE` → open **down at upper neighbor**
3. **randomDfs(start)** — push `start`; while stack not empty:
    - peek `pos`
    - `delta = nextCell(pos)`
    - if `delta == 0` → backtrack (`pop_back`)
    - else `next = pos + delta`; **connect(pos,next);** mark visited; `push_back(next)`
4. **createMazeDfs()** — reset `visited` and `walls` to false, `std::srand(std::time(nullptr))`, then `randomDfs(startPos)`.

**Design notes**
  - **No recursion** → avoids stack-overflow for larger grids.
  - **Determinism:** use a fixed seed if you need reproducible mazes.
  - External readers (`rightOpen/downOpen` by index or `(x,y)`) expose connectivity to the renderer and collision system.

---

## Rendering with Raylib

**Coordinates & sizing**

* Tunables: `CELL`, `WALL_THICK`, `WALL_HEIGHT`, `FLOOR_THICK`, `GROUND_Y`
* `ORIGIN` centers the maze around world `(0,0,0)`

**Helpers**

* `centerX(i)` / `centerZ(j)` → world-space cell centers
* `edgeX(i)` / `edgeZ(j)` → seam/edge positions for precise wall placement

**Walls**

* **Textured path:** pre-load one cube mesh → model, assign a brick texture; draw via `DrawModelEx` with per-instance scaling (`sx`, `sy`, `sz`).
* **Fallback:** if texture/model not ready, draw via `DrawCube` + `DrawCubeWires` (keeps visuals legible even without assets).

**Lifecycle**

* `build()` → generate maze, load texture, generate model, build AABB collisions, set `built=true`.
* `rebuild()` → regenerate maze & collisions without reloading assets (fast restart).
* `unload()` → free model/texture and reset flags.

**Start/Goal tiles**

* Thin colored plates: **Start** (GREEN) at `startPos`, **Goal** (PURPLE) at `(SIZE-1, SIZE-1)`.

**Draw order**

1. Start tile
2. Outer **left** and **top** borders (always present)
3. Interior walls: for each cell → draw **right** wall if edge or `!rightOpen`; draw **bottom** wall if edge or `!downOpen`
4. Goal tile

**Collisions (AABB)**

* Precompute a `std::vector<BoundingBox>` aligned **1:1** with visible walls.
* Always add the **outer left/top** frame; then, for each cell:

  * add **right** wall collider if on the outer edge or `!rightOpen(i, j)`
  * add **bottom** wall collider if on the outer edge or `!downOpen(i, j)`
* Expose via `colliders()` so the player controller / physics can use the same authoritative wall set as the renderer.

---

## Build & Run

**Prerequisites**

* `raylib` installed (or vendored)
* C++17 or later (tested with `g++`/`clang++`)

**Windows (Visual Studio 2022)**

* Open the provided `.sln` in **Visual Studio 2022** and build (x64 Debug/Release). Configure `raylib` include/lib paths if needed (Project → Properties → C/C++ and Linker).

**Windows (MinGW/MSYS2)**

```bash
g++ -std=c++17 -O2 -I/include -L/lib \
  src/main.cpp src/Maze.cpp src/Maze3D.cpp -o MazeEscapeGame \
  -lraylib -lopengl32 -lgdi32 -lwinmm
```

**Linux**

```bash
g++ -std=c++17 -O2 src/main.cpp src/Maze.cpp src/Maze3D.cpp -o MazeEscapeGame \
  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

**macOS**

```bash
clang++ -std=c++17 -O2 src/main.cpp src/Maze.cpp src/Maze3D.cpp -o MazeEscapeGame \
  -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
```

*If you keep assets like `brick.jpg`, place them under `assets/` and load with a relative path.*
